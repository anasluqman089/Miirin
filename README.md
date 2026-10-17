# Miirin

Miirin is a compact MP3 player built around a Seeed XIAO ESP32-S3 and a
DFPlayer Pro. It has an OLED display, five buttons, a rotary encoder, and an
RGB status LED. The `CAD`, `PCB`, and `Production` folders contain the
enclosure, board, and manufacturing files.

## Bill of materials


| Qty. | Reference | Part | Price ($) | Buy links |
| ---: | --- | --- | ---: | --- |
| 1 | U1 | Seeed XIAO ESP32-S3 module | $21.11 | [AliExpress](https://www.aliexpress.com/item/1005010765493617.html) |
| 1 | U2 | DFRobot DFPlayer Pro (DFR0768) | $12.47 | [AliExpress](https://www.aliexpress.com/item/1005009801181233.html) |
| 1 | - | 1.3 Inch OLED screen | $5.97 | [AliExpress](https://www.aliexpress.com/item/1005012014098105.html) |
| 1 | U3 | PCF8574P I2C I/O expander | $4.56 | [AliExpress](https://www.aliexpress.com/item/1005010617692092.html) (sold in 5pcs) |
| 1 | D1 | Common-anode RGB LED | $2.00 | [AliExpress](https://www.aliexpress.com/item/1005012188300715.html) (sold in 10pcs) |
| 1 | Q1 | P-channel MOSFET | $8.45 | [AliExpress](https://www.aliexpress.com/item/1005011589388936.html) (sold in 5pcs) |
| 1 | SW6 | Rotary encoder | $0.77 | [AliExpress](https://www.aliexpress.com/item/1005006127287538.html) |
| 5 | SW1-SW5 | Momentary push buttons | $0.87 | [AliExpress](https://www.aliexpress.com/item/1005009251091250.html) (sold in 5pcs) |
| 1 | J1 | 4-pin socket/header for the OLED | $0.93 | [AliExpress](https://www.aliexpress.com/item/1005007093958549.html) |
| 1 | J2 | Switched 3.5 mm audio jack | $1.99 | [AliExpress](https://www.aliexpress.com/item/1005012057262599.html) (sold in 10pcs) |
| 3 | R4-R6 | 220 Ω resistors | $1.00 | [AliExpress](https://www.aliexpress.com/item/1005011886443911.html) (sold in 20pcs) |
| 2 | R2-R3 | 4.7 kΩ resistors | $1.00 | [AliExpress](https://www.aliexpress.com/item/1005011886443911.html) (sold in 20 pcs) |
| 1 | R1 | 100 kΩ resistor | $1.00 | [AliExpress](https://www.aliexpress.com/item/1005011886443911.html) (sold in 20 pcs) |
| 2 | - | 100 kΩ resistors | $1.00 | [AliExpress](https://www.aliexpress.com/item/1005011886443911.html) (sold in 20 pcs) |
| 2 | C1-C2 | 10 µF ceramic capacitors | $0.77 | [AliExpress](https://www.aliexpress.com/item/1005004319296106.html) (sold in 50pcs) |
| 1 | C4 | 0.1 µF ceramic capacitor | $0.77 | [AliExpress](https://www.aliexpress.com/item/1005004319296106.html) (sold in 50pcs) |
| 1 | C5 | 470 µF polarized capacitor | $2.97 | [AliExpress](https://www.aliexpress.com/item/1005012167135628.html) |
| 4 | H1-H4 | M2 screw | - | - |
| 4 | - | M3 screw | - | - |
| 1 | - | PCB | $4.00 | - |
| 4 | - | 3D print | ~$7.00 | - |

**Estimated total** = **$56.73** *Not inluding shipping

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


