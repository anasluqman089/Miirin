# Miirin

Miirin is a compact MP3 player built around a Seeed XIAO ESP32-S3 and a
DFPlayer Pro. It has an OLED display, five buttons, a rotary encoder, and an
RGB status LED. The `CAD`, `PCB`, and `Production` folders contain the
enclosure, board, and manufacturing files.

## Getting started

Open `Firmware/Miirin.ino` in the Arduino IDE. Install Espressif's **esp32**
board package and the **U8g2** and **DFRobot_DF1201S** libraries, then select **XIAO_ESP32S3** as the board. Connect the XIAO, verify the sketch, and upload
it.

Put your audio files on the DFPlayer Pro's storage. At startup, the player
checks how many tracks are available and restores the last selected track and
volume while the XIAO remains powered.

## Controls and display

- **SW1:** Play or pause the current track.
- **SW2 / SW3:** Select the previous or next track. Selection wraps at either
  end of the list.
- **SW4:** Play the selected track.
- **Rotary encoder:** Adjust volume.
- **Hold SW5:** Pause playback and put the device into deep sleep. Press SW5 to
  wake it.

The OLED shows the current track, playback selection, volume, and battery
percentage. Battery percentage is an estimate based on cell voltage, not a
fuel-gauge measurement.

## Battery and RGB LED

Battery monitoring uses the divider wired to **D0**: battery positive goes through a 100 kΩ resistor to D0, then another 100 kΩ resistor connects D0 to battery negative (ground). The battery itself connects to the XIAO's battery pads.

The RGB LED is driven through the PCF8574 and indicates battery level:

- **Red:** Battery is at or below 20%
- **Green:** Battery is above 20%

Charging is not identified separately because the board has no charge-status
signal connected. The PCF8574 interrupt is connected to **D10**.

## Bill of materials

| Qty. | Reference(s) | Part |
| ---: | --- | --- |
| 1 | U1 | Seeed XIAO ESP32-S3 module |
| 1 | U2 | DFRobot DFPlayer Pro (DFR0768) |
| 1 | - | 1.3 Inch OLED screen|
| 1 | U3 | PCF8574P I2C I/O expander |
| 1 | D1 | Common-anode RGB LED |
| 1 | Q1 | P-channel MOSFET |
| 1 | SW6 | Rotary encoder |
| 5 | SW1-SW5 | Momentary push buttons |
| 1 | J1 | 4-pin socket/header for the OLED |
| 1 | J2 | Switched 3.5 mm audio jack |
| 3 | R4-R6 | 220 Ω resistors |
| 2 | R2-R3 | 4.7 kΩ resistors |
| 1 | R1 | 100 kΩ resistor |
| 2 | C1-C2 | 10 µF ceramic capacitors |
| 1 | C4 | 0.1 µF ceramic capacitor |
| 1 | C5 | 470 µF polarized capacitor |
| 4 | H1-H4 | M2 screw |
| 1 | - | PCB |
| 4 | - | 3D print |

The battery-sense divider described above is external to the current PCB
schematic. It needs two additional 100 kΩ resistors between battery positive,
D0, and battery negative. They have no PCB reference designators yet and are
not included in the on-board quantities above. The battery, OLED, and
headphones are also external to the PCB BOM.
