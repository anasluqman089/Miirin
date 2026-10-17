# Miirin

Miirin is a compact MP3 player built around a Seeed XIAO ESP32-S3 and a
DFPlayer Pro. It has an OLED display, five buttons, a rotary encoder, and an
RGB status LED. The `CAD`, `PCB`, and `Production` folders contain the
enclosure, board, and manufacturing files.

## Bill of materials

| Qty. | Reference(s) | Part | Est. unit price (USD) | Est. line total (USD) | Buy / search |
| ---: | --- | --- | ---: | ---: | --- |
| 1 | U1 | Seeed XIAO ESP32-S3 module | $7.99 | $7.99 | [Seeed](https://www.seeedstudio.com/XIAO-ESP32S3-p-5627.html) |
| 1 | U2 | DFRobot DFPlayer Pro (DFR0768) | $22.00 | $22.00 | [DFRobot search](https://www.dfrobot.com/search?keywords=DFR0768) |
| 1 | - | 1.3 Inch OLED screen | $8.00 | $8.00 | [Amazon search](https://www.amazon.com/s?k=1.3+inch+OLED+I2C+128x64) |
| 1 | U3 | PCF8574P I2C I/O expander | $1.50 | $1.50 | [Mouser search](https://www.mouser.com/c/?q=PCF8574P) |
| 1 | D1 | Common-anode RGB LED | $0.50 | $0.50 | [Mouser search](https://www.mouser.com/c/?q=common%20anode%20RGB%20LED%20through%20hole) |
| 1 | Q1 | P-channel MOSFET | $0.25 | $0.25 | [Mouser search](https://www.mouser.com/c/?q=P-channel%20MOSFET) |
| 1 | SW6 | Rotary encoder | $1.50 | $1.50 | [Amazon search](https://www.amazon.com/s?k=rotary+encoder+through+hole) |
| 5 | SW1-SW5 | Momentary push buttons | $0.15 | $0.75 | [Mouser search](https://www.mouser.com/c/?q=tactile%20switch%20through%20hole) |
| 1 | J1 | 4-pin socket/header for the OLED | $0.50 | $0.50 | [Amazon search](https://www.amazon.com/s?k=4+pin+2.54mm+header+socket) |
| 1 | J2 | Switched 3.5 mm audio jack | $1.50 | $1.50 | [Mouser search](https://www.mouser.com/c/?q=switched%203.5mm%20audio%20jack%20through%20hole) |
| 3 | R4-R6 | 220 Ω resistors | $0.03 | $0.09 | [Mouser search](https://www.mouser.com/c/?q=220%20ohm%20resistor%201%2F4%20W) |
| 2 | R2-R3 | 4.7 kΩ resistors | $0.03 | $0.06 | [Mouser search](https://www.mouser.com/c/?q=4.7k%20ohm%20resistor%201%2F4%20W) |
| 1 | R1 | 100 kΩ resistor | $0.03 | $0.03 | [Mouser search](https://www.mouser.com/c/?q=100k%20ohm%20resistor%201%2F4%20W) |
| 2 | - | 100 kΩ resistors for the external battery-sense divider | $0.03 | $0.06 | [Mouser search](https://www.mouser.com/c/?q=100k%20ohm%20resistor%201%2F4%20W) |
| 2 | C1-C2 | 10 µF ceramic capacitors | $0.15 | $0.30 | [Mouser search](https://www.mouser.com/c/?q=10uF%20ceramic%20capacitor) |
| 1 | C4 | 0.1 µF ceramic capacitor | $0.05 | $0.05 | [Mouser search](https://www.mouser.com/c/?q=0.1uF%20ceramic%20capacitor) |
| 1 | C5 | 470 µF polarized capacitor | $0.25 | $0.25 | [Mouser search](https://www.mouser.com/c/?q=470uF%20electrolytic%20capacitor) |
| 4 | H1-H4 | M2 screw | $0.10 | $0.40 | [Amazon search](https://www.amazon.com/s?k=M2+screws) |
| 1 | - | PCB | $5.00 | $5.00 | [PCBWay order](https://www.pcbway.com/orderonline.aspx) |
| 4 | - | 3D print | $1.50 | $6.00 | [PCBWay 3D printing](https://www.pcbway.com/rapid-prototyping/3d-printing/) |

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


