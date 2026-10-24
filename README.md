# Miirin

Miirin is a compact MP3 player built around a Seeed XIAO ESP32-S3 and a
DFPlayer Pro. It has an OLED display, five buttons, a rotary encoder, and an
RGB status LED. The `CAD`, `PCB`, and `Production` folders contain the
enclosure, board, and manufacturing files.

## Bill of materials


# Bill of Materials

# Bill of Materials

| Quantity | Reference | Part | Prices ($) | Buy Links |
| :---: | :---: | :--- | :---: | :--- |
| 1 | U1 | Seeed XIAO ESP32-S3 module | 7.50 | [AliExpress](https://www.aliexpress.com/w/wholesale-Seeed-XIAO-ESP32-S3-module.html) |
| 1 | U2 | DFRobot DFPlayer Pro (DFR0768) | 8.50 | [AliExpress](https://www.aliexpress.com/w/wholesale-DFRobot-DFPlayer-Pro-DFR0768.html) |
| 1 | - | 1.3 Inch OLED screen | 3.50 | [AliExpress](https://www.aliexpress.com/w/wholesale-1.3-Inch-OLED-screen.html) |
| 1 | U3 | PCF8574P I2C I/O expander | 1.20 | [AliExpress](https://www.aliexpress.com/w/wholesale-PCF8574P-I2C-IO-expander.html) |
| 1 | D1 | Common-anode RGB LED | 0.35 | [AliExpress](https://www.aliexpress.com/w/wholesale-Common-anode-RGB-LED.html) |
| 1 | Q1 | P-channel MOSFET | 0.80 | [AliExpress](https://www.aliexpress.com/w/wholesale-P-channel-MOSFET.html) |
| 1 | SW6 | Rotary encoder | 0.60 | [AliExpress](https://www.aliexpress.com/w/wholesale-Rotary-encoder.html) |
| 1 | SW5 | Momentary push buttons | 0.15 | [AliExpress](https://www.aliexpress.com/w/wholesale-Momentary-push-buttons.html) |
| 4 | SW1-SW4 | Cherry MX Keyboard Switches | 2.00 | [AliExpress](https://www.aliexpress.com/w/wholesale-Cherry-MX-switch.html) |
| 1 | J1 | 4-pin socket/header for the OLED | 0.10 | [AliExpress](https://www.aliexpress.com/w/wholesale-4-pin-socket-header-for-the-OLED.html) |
| 1 | J2 | Switched 3.5 mm audio jack | 0.50 | [AliExpress](https://www.aliexpress.com/w/wholesale-Switched-3.5-mm-audio-jack.html) |
| 3 | R4-R6 | 220 Ohm resistors | 0.15 | [AliExpress](https://www.aliexpress.com/w/wholesale-220-Ohm-resistors.html) |
| 2 | R2-R3 | 4.7 kOhm resistors | 0.10 | [AliExpress](https://www.aliexpress.com/w/wholesale-4.7-kOhm-resistors.html) |
| 1 | R1 | 100 kOhm resistor | 0.05 | [AliExpress](https://www.aliexpress.com/w/wholesale-100-kOhm-resistor.html) |
| 2 | - | 100 kOhm resistors | 0.10 | [AliExpress](https://www.aliexpress.com/w/wholesale-100-kOhm-resistors.html) |
| 2 | C1-C2 | 10 uF ceramic capacitors | 0.30 | [AliExpress](https://www.aliexpress.com/w/wholesale-10-uF-ceramic-capacitors.html) |
| 1 | C4 | 0.1 uF ceramic capacitor | 0.10 | [AliExpress](https://www.aliexpress.com/w/wholesale-0.1-uF-ceramic-capacitor.html) |
| 1 | C5 | 470 uF polarized capacitor | 0.45 | [AliExpress](https://www.aliexpress.com/w/wholesale-470-uF-polarized-capacitor.html) |
| 3 | H1-H3 | M2 screw | 0.30 | [AliExpress](https://www.aliexpress.com/w/wholesale-M2-screw.html) |
| 5 | - | M3 screw | 0.50 | [AliExpress](https://www.aliexpress.com/w/wholesale-M3-screw.html) |
| 1 | - | PCB | 8.60 | - |
| 4 | - | 3D print | ~10.00 | - |
| **Total** | | | **$45.85** Not inluding shipping | |

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

## Uploading code & audio files

Upload the code in firmware folder. Make sure to select the XIAO ESP32 S3 board.

Put your audio files on the DFPlayer Pro's storage. At startup, the player checks how many tracks are available and restores the last selected track and volume while the XIAO remains powered.

## CAD

Here is the link for the CAD
[Onshape](https://cad.onshape.com/documents/3d8e9b5928a29fef03f51911/w/7d3ee8411c85baeb3f6a489e/e/28521f022b7deb8d589466ae?renderMode=0&uiState=6ad3208a288b1a9cd259184f)

## Images

### Schematic

<img width="760" height="558" alt="image" src="https://github.com/user-attachments/assets/835f4810-9a1a-4945-b640-ffc196da6095" />

### PCB

<img width="430" height="510" alt="image" src="https://github.com/user-attachments/assets/b269de17-2eaa-43e4-9293-7ae1a2f84094" />

### CAD

<img width="764" height="463" alt="image" src="https://github.com/user-attachments/assets/7e6d2534-1828-4ccb-b33b-a7d2fd628de0" />


