# Hardware Firmware

This folder contains the ESP32 firmware retrieved from the project Google Drive and sanitized for public GitHub use.

## Before uploading
Edit these placeholders in `smart_laundry_hardware.ino`:

```cpp
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
const char* ACCESS_TOKEN = "YOUR_THINGSBOARD_ACCESS_TOKEN";
```

Do **not** commit real Wi-Fi passwords or ThingsBoard access tokens to a public repository.

## Main hardware mapping

| Function | ESP32 GPIO |
|---|---:|
| Relay 1 | 13 |
| Relay 2 | 14 |
| Relay 3 | 27 |
| Relay 4 | 26 |
| Button 1 | 21 |
| Button 2 | 22 |
| Button 3 | 15 |
| Button 4 | 12 |
| Buzzer | 25 |
| TFT CS | 5 |
| TFT RST | 4 |
| TFT DC | 2 |
| TFT MOSI | 23 |
| TFT SCK | 18 |

The hardware firmware in this repository uses an ST7735 TFT display, matching the physical prototype code from Drive.
