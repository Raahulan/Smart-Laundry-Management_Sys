# Pin Mapping

## ESP32 outputs

| Channel | GPIO | Purpose |
|---|---:|---|
| Relay 1 | 13 | Washing machine channel 1 |
| Relay 2 | 14 | Washing machine channel 2 |
| Relay 3 | 27 | Washing machine channel 3 |
| Relay 4 | 26 | Washing machine channel 4 |
| Buzzer | 25 | Local alert output |

## Push-button inputs

| Button | GPIO |
|---|---:|
| Button 1 | 21 |
| Button 2 | 22 |
| Button 3 | 15 |
| Button 4 | 12 |

Buttons use `INPUT_PULLUP`, so the normal state is HIGH and a press pulls the input LOW.

## TFT

| TFT signal | ESP32 GPIO |
|---|---:|
| CS | 5 |
| RST | 4 |
| DC / A0 | 2 |
| MOSI | 23 |
| SCK | 18 |

The physical prototype firmware uses an ST7735 display. The Wokwi simulation evidence includes an ILI9341 display version used during simulation development.
