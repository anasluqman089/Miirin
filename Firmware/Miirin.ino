#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <DFRobot_DF1201S.h>
#include <esp_sleep.h>
#include <driver/rtc_io.h>

#define OLED_IS_SH1106 1
#define BATTERY_ADC_PIN D0
#define CHARGING_STATUS_PIN -1

constexpr uint8_t PCF8574_ADDRESS = 0x20; 
constexpr uint8_t PCF_INT_PIN = D10;      
constexpr uint8_t ENCODER_A_PIN = D1;
constexpr uint8_t ENCODER_B_PIN = D2;
constexpr uint8_t OLED_SDA_PIN = D4;
constexpr uint8_t OLED_SCL_PIN = D5;
constexpr uint8_t AUDIO_TX_PIN = D6;
constexpr uint8_t AUDIO_RX_PIN = D7;
constexpr uint8_t PLAYER_POWER_PIN = D3;
constexpr uint8_t PLAYER_POWER_ON_LEVEL = LOW;  
const uint8_t BUTTON_BITS[] = {7, 6, 5, 4, 3};
constexpr uint8_t BUTTON_COUNT = sizeof(BUTTON_BITS) / sizeof(BUTTON_BITS[0]);
constexpr uint8_t LED_RED_BIT = 0;
constexpr uint8_t LED_GREEN_BIT = 1;
constexpr uint8_t LED_BLUE_BIT = 2;
constexpr uint8_t DEFAULT_VOLUME = 15;
constexpr uint8_t MAX_VOLUME = 30;
constexpr uint32_t DEEP_SLEEP_HOLD_MS = 1200;
constexpr uint32_t DEBOUNCE_MS = 25;
constexpr float BATTERY_DIVIDER_RATIO = 2.0f;
constexpr bool CHARGING_STATUS_ACTIVE_LOW = true;

#if OLED_IS_SH1106
U8G2_SH1106_128X64_NONAME_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);
#else
U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);
#endif

HardwareSerial audioSerial(1);
DFRobot_DF1201S player;

RTC_DATA_ATTR uint16_t rememberedTrack = 1;
RTC_DATA_ATTR uint8_t rememberedVolume = DEFAULT_VOLUME;

struct ButtonState {
  bool stablePressed = false;
  bool previousReading = false;
  bool pressedEvent = false;
  uint32_t changedAt = 0;
  uint32_t pressedAt = 0;
};

ButtonState buttons[BUTTON_COUNT];
uint8_t pcfOutput = 0xFF;
uint16_t uploadedTrackCount = 0;
uint16_t selectedTrack = 1;
uint16_t currentPlayingTrack = 0;
uint8_t volumeLevel = DEFAULT_VOLUME;
bool playerReady = false;
bool currentlyPlaying = false;
bool screenDirty = true;
bool trackNameLookupPending = false;
char currentTrackName[40] = "No track playing";
uint32_t trackNameRequestAt = 0;
uint32_t lastButtonPoll = 0;
uint32_t lastScreenUpdate = 0;
uint8_t previousEncoderState = 0;
int8_t encoderSteps = 0;

const int8_t ENCODER_TRANSITIONS[16] = {
   0, -1,  1,  0,
   1,  0,  0, -1,
  -1,  0,  0,  1,
   0,  1, -1,  0
};

bool writePcf(uint8_t value) {
  Wire.beginTransmission(PCF8574_ADDRESS);
  Wire.write(value);
  return Wire.endTransmission() == 0;
}

uint8_t readPcf() {
  if (Wire.requestFrom(PCF8574_ADDRESS, static_cast<uint8_t>(1)) != 1) {
    return 0xFF;
  }
  return Wire.read();
}

void setRgb(bool red, bool green, bool blue) {
  pcfOutput = 0xFF;
  if (red) pcfOutput &= ~(1U << LED_RED_BIT);
  if (green) pcfOutput &= ~(1U << LED_GREEN_BIT);
  if (blue) pcfOutput &= ~(1U << LED_BLUE_BIT);
  writePcf(pcfOutput);
}

int readBatteryPercent() {
#if BATTERY_ADC_PIN >= 0
  uint32_t sumMillivolts = 0;
  for (uint8_t sample = 0; sample < 8; ++sample) {
    sumMillivolts += analogReadMilliVolts(BATTERY_ADC_PIN);
  }
  const uint16_t batteryMillivolts = static_cast<uint16_t>(
      (sumMillivolts / 8.0f) * BATTERY_DIVIDER_RATIO);

  const uint16_t voltageTable[] = {3000, 3300, 3500, 3600, 3700, 3800, 3900, 4000, 4100, 4200};
  const uint8_t percentTable[] = {0, 5, 10, 20, 35, 50, 65, 80, 90, 100};
  constexpr uint8_t points = sizeof(voltageTable) / sizeof(voltageTable[0]);

  if (batteryMillivolts <= voltageTable[0]) return 0;
  if (batteryMillivolts >= voltageTable[points - 1]) return 100;
  for (uint8_t index = 1; index < points; ++index) {
    if (batteryMillivolts <= voltageTable[index]) {
      const uint16_t lowVoltage = voltageTable[index - 1];
      const uint16_t highVoltage = voltageTable[index];
      const uint8_t lowPercent = percentTable[index - 1];
      const uint8_t highPercent = percentTable[index];
      return lowPercent + ((batteryMillivolts - lowVoltage) *
                           (highPercent - lowPercent)) /
                              (highVoltage - lowVoltage);
    }
  }
#endif
  return -1;
}

bool isCharging() {
#if CHARGING_STATUS_PIN >= 0
  const bool pinHigh = digitalRead(CHARGING_STATUS_PIN) == HIGH;
  return CHARGING_STATUS_ACTIVE_LOW ? !pinHigh : pinHigh;
#else
  return false;
#endif
}

void updateBatteryLed() {
  if (isCharging()) {
    setRgb(false, false, true);
    return;
  }

  const int percent = readBatteryPercent();
  if (percent < 0) {
    setRgb(false, false, false);
  } else if (percent <= 20) {
    setRgb(true, false, false);
  } else {
    setRgb(false, true, false);
  }
}

void drawScreen() {
  display.clearBuffer();
  display.setFont(u8g2_font_6x10_tf);
  display.drawUTF8(0, 9, currentTrackName);

  char status[24];
  const int batteryPercent = readBatteryPercent();
  if (isCharging()) {
    snprintf(status, sizeof(status), "Charging  Vol %u", static_cast<unsigned>(volumeLevel));
  } else if (batteryPercent >= 0) {
    snprintf(status, sizeof(status), "Bat %d%%  Vol %u", batteryPercent,
             static_cast<unsigned>(volumeLevel));
  } else {
    snprintf(status, sizeof(status), "Bat --  Vol %u", static_cast<unsigned>(volumeLevel));
  }
  display.drawStr(0, 21, status);
  display.drawHLine(0, 24, 128);

  const uint8_t visibleRows = 4;
  if (uploadedTrackCount == 0) {
    display.drawStr(0, 42, "No files found");
    display.sendBuffer();
    screenDirty = false;
    lastScreenUpdate = millis();
    return;
  }

  uint16_t firstTrack = selectedTrack > 2 ? selectedTrack - 2 : 1;
  if (firstTrack + visibleRows - 1 > uploadedTrackCount && uploadedTrackCount >= visibleRows) {
    firstTrack = uploadedTrackCount - visibleRows + 1;
  }

  for (uint8_t row = 0; row < visibleRows; ++row) {
    const uint16_t trackNumber = firstTrack + row;
    if (trackNumber > uploadedTrackCount) break;
    const uint8_t baseline = 34 + row * 9;
    if (trackNumber == selectedTrack) {
      display.drawBox(0, baseline - 8, 128, 9);
      display.setDrawColor(0);
    }
    char trackLabel[16];
    snprintf(trackLabel, sizeof(trackLabel), "%c %03u",
             trackNumber == selectedTrack ? '>' : ' ',
             static_cast<unsigned>(trackNumber));
    display.drawStr(2, baseline, trackLabel);
    if (currentlyPlaying && trackNumber == currentPlayingTrack) {
      display.drawStr(112, baseline, "PLAY");
    }
    display.setDrawColor(1);
  }

  display.sendBuffer();
  screenDirty = false;
  lastScreenUpdate = millis();
}

void playSelectedTrack() {
  if (!playerReady || selectedTrack == 0 || selectedTrack > uploadedTrackCount) return;
  player.playFileNum(selectedTrack);
  currentlyPlaying = true;
  currentPlayingTrack = selectedTrack;
  rememberedTrack = selectedTrack;
  snprintf(currentTrackName, sizeof(currentTrackName), "Track %u",
           static_cast<unsigned>(currentPlayingTrack));
  trackNameLookupPending = true;
  trackNameRequestAt = millis();
  screenDirty = true;
}

void togglePlayback() {
  if (!playerReady) return;
  if (currentlyPlaying) {
    player.pause();
    currentlyPlaying = false;
  } else {
    if (currentPlayingTrack == selectedTrack && currentPlayingTrack != 0) {
      player.start();
      currentlyPlaying = true;
    } else {
      playSelectedTrack();
    }
  }
  screenDirty = true;
}

void changeSelection(int8_t amount) {
  if (uploadedTrackCount == 0) return;
  if (amount < 0) {
    selectedTrack = selectedTrack <= 1 ? uploadedTrackCount : selectedTrack - 1;
  } else {
    selectedTrack = selectedTrack >= uploadedTrackCount ? 1 : selectedTrack + 1;
  }
  rememberedTrack = selectedTrack;
  screenDirty = true;
}

void changeVolume(int8_t amount) {
  int16_t next = static_cast<int16_t>(volumeLevel) + amount;
  if (next < 0) next = 0;
  if (next > MAX_VOLUME) next = MAX_VOLUME;
  if (next == volumeLevel) return;
  volumeLevel = static_cast<uint8_t>(next);
  rememberedVolume = volumeLevel;
  if (playerReady) player.setVol(volumeLevel);
  screenDirty = true;
}

void pollButtons() {
  const uint32_t now = millis();
  if (now - lastButtonPoll < 5) return;
  lastButtonPoll = now;

  const uint8_t portValue = readPcf();
  for (uint8_t index = 0; index < BUTTON_COUNT; ++index) {
    const bool readingPressed = (portValue & (1U << BUTTON_BITS[index])) == 0;
    ButtonState &button = buttons[index];
    button.pressedEvent = false;

    if (readingPressed != button.previousReading) {
      button.previousReading = readingPressed;
      button.changedAt = now;
    }
    if (readingPressed != button.stablePressed && now - button.changedAt >= DEBOUNCE_MS) {
      button.stablePressed = readingPressed;
      if (readingPressed) {
        button.pressedAt = now;
        button.pressedEvent = true;
      }
    }
  }
}

uint8_t readEncoderState() {
  return (digitalRead(ENCODER_A_PIN) << 1) | digitalRead(ENCODER_B_PIN);
}

void pollEncoder() {
  const uint8_t currentState = readEncoderState();
  const uint8_t transition = (previousEncoderState << 2) | currentState;
  encoderSteps += ENCODER_TRANSITIONS[transition];
  previousEncoderState = currentState;

  if (encoderSteps >= 4) {
    changeVolume(1);
    encoderSteps = 0;
  } else if (encoderSteps <= -4) {
    changeVolume(-1);
    encoderSteps = 0;
  }
}

void enterDeepSleep() {
  if (playerReady) player.pause();
  currentlyPlaying = false;
  digitalWrite(PLAYER_POWER_PIN, PLAYER_POWER_ON_LEVEL == LOW ? HIGH : LOW);
  setRgb(false, false, false);
  display.setPowerSave(1);

  // Release SW5 and read the expander once so its active-low INT line returns high.
  while ((readPcf() & (1U << BUTTON_BITS[4])) == 0) {
    delay(10);
  }
  readPcf();

  pinMode(PCF_INT_PIN, INPUT_PULLUP);
  rtc_gpio_pullup_en(static_cast<gpio_num_t>(PCF_INT_PIN));
  rtc_gpio_pulldown_dis(static_cast<gpio_num_t>(PCF_INT_PIN));
  esp_sleep_enable_ext0_wakeup(static_cast<gpio_num_t>(PCF_INT_PIN), 0);
  Serial.println("Entering deep sleep. Press SW5 to wake.");
  Serial.flush();
  esp_deep_sleep_start();
}

void handleButtonEvents() {
  if (buttons[0].pressedEvent) togglePlayback();
  if (buttons[1].pressedEvent) changeSelection(-1);
  if (buttons[2].pressedEvent) changeSelection(1);
  if (buttons[3].pressedEvent) playSelectedTrack();

  if (buttons[4].stablePressed && millis() - buttons[4].pressedAt >= DEEP_SLEEP_HOLD_MS) {
    enterDeepSleep();
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);
  Wire.setClock(100000);

  volumeLevel = rememberedVolume <= MAX_VOLUME ? rememberedVolume : DEFAULT_VOLUME;

  pcfOutput = 0xFF;
  if (!writePcf(pcfOutput)) {
    Serial.println("PCF8574 not found at I2C address 0x20.");
  }
  const uint8_t initialPort = readPcf();
  for (uint8_t index = 0; index < BUTTON_COUNT; ++index) {
    buttons[index].previousReading = (initialPort & (1U << BUTTON_BITS[index])) == 0;
  }

  pinMode(ENCODER_A_PIN, INPUT_PULLUP);
  pinMode(ENCODER_B_PIN, INPUT_PULLUP);
  previousEncoderState = readEncoderState();

  pinMode(PLAYER_POWER_PIN, OUTPUT);
  digitalWrite(PLAYER_POWER_PIN, PLAYER_POWER_ON_LEVEL == LOW ? HIGH : LOW);
  delay(20);
  digitalWrite(PLAYER_POWER_PIN, PLAYER_POWER_ON_LEVEL);
  delay(1000);

#if CHARGING_STATUS_PIN >= 0
  pinMode(CHARGING_STATUS_PIN,
          CHARGING_STATUS_ACTIVE_LOW ? INPUT_PULLUP : INPUT_PULLDOWN);
#endif
#if BATTERY_ADC_PIN >= 0
  analogReadResolution(12);
  analogSetPinAttenuation(BATTERY_ADC_PIN, ADC_11db);
#endif

  display.setI2CAddress(0x3C * 2);
  display.begin();

  audioSerial.begin(115200, SERIAL_8N1, AUDIO_RX_PIN, AUDIO_TX_PIN);
  for (uint8_t attempt = 0; attempt < 5 && !playerReady; ++attempt) {
    playerReady = player.begin(audioSerial);
    if (!playerReady) delay(500);
  }
  if (playerReady) {
    player.switchFunction(player.MUSIC);
    player.setPrompt(false);
    player.setPlayMode(player.SINGLE);
    player.setVol(volumeLevel);
    delay(200);
    uploadedTrackCount = player.getTotalFile();
    selectedTrack = uploadedTrackCount > 0 && rememberedTrack >= 1 &&
                            rememberedTrack <= uploadedTrackCount
                        ? rememberedTrack
                        : (uploadedTrackCount > 0 ? 1 : 0);
  } else {
    Serial.println("DFPlayer Pro not responding. Check UART wiring and power.");
    selectedTrack = 0;
  }

  updateBatteryLed();
  drawScreen();
}

void loop() {
  pollButtons();
  pollEncoder();
  handleButtonEvents();

  if (playerReady && trackNameLookupPending && millis() - trackNameRequestAt >= 400) {
    String trackName = player.getFileName();
    trackName.trim();
    if (trackName.length() > 0) {
      trackName.toCharArray(currentTrackName, sizeof(currentTrackName));
    }
    trackNameLookupPending = false;
    screenDirty = true;
  }

  static uint32_t lastBatteryUpdate = 0;
  if (millis() - lastBatteryUpdate >= 2000) {
    updateBatteryLed();
    lastBatteryUpdate = millis();
    screenDirty = true;
  }

  if (screenDirty && millis() - lastScreenUpdate >= 100) {
    drawScreen();
  }
}