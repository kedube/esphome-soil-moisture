# ESPHome Soil Moisture Sensor
ESPHome configuration for a capacitive soil moisture sensor running on either an **ESP32-C6-Zero** or a **WeMos D1 mini Pro**.

This project exposes a calibrated soil moisture percentage, a diagnostic voltage reading, Wi-Fi diagnostics, and a simple local web UI. It is designed to be easy to flash, adjust, and integrate with Home Assistant.

## Supported Devices

| Device | ESPHome Entry File | Notes |
| :--- | :--- | :--- |
| ESP32-C6-Zero | `esphome/soil-moisture.yaml` | Default configuration, ESP-IDF build, GPIO0 analog input, WS2812 RGB status LED on GPIO8, Wi-Fi protocol diagnostic sensor |
| WeMos D1 mini Pro | `esphome/soil-moisture.yaml` | Uses the same main YAML after switching the settings and device package includes to the WeMos variants |

## Repository Layout

- `esphome/soil-moisture.yaml` is the main ESPHome entry file.
- `esphome/settings.yaml` contains ESP32-C6 substitutions such as device name, update interval, GPIO selection, and moisture calibration voltages.
- `esphome/settings_wemosd1.yaml` contains the WeMos D1 mini Pro substitutions and defaults.
- `esphome/packages/device_esp32c6.yaml` contains the ESP32-C6-specific board settings and status LED behavior.
- `esphome/packages/device_wemosd1.yaml` contains the ESP8266 / WeMos D1 mini Pro board settings and ADC configuration.
- `esphome/packages/shared_helpers.yaml` contains shared diagnostic sensors and text sensors.
- `esphome/examples/secrets-example.yaml` contains the template for the secrets file used by the main configuration.
- `include/status_led_helper.h` contains the status LED state logic.
- `include/wifi_protocol_helper.h` contains the Wi-Fi protocol helper used by the diagnostic text sensor.

## Features

- ESPHome integration for Home Assistant
- Local web interface with authentication
- OTA firmware updates
- Soil moisture percentage derived from sensor voltage
- Diagnostic `Soil Moisture Voltage` sensor for calibration
- User-facing `Soil Moisture Percentage` sensor derived from configurable dry and wet voltages
- Wi-Fi signal, SSID, IP address, uptime, and firmware version sensors
- Wi-Fi protocol diagnostic text sensor on the ESP32-C6 build, powered by `include/wifi_protocol_helper.h`
- Status LED support on both platforms

## Current Configuration

### ESP32-C6 Configuration

- Main file: `esphome/soil-moisture.yaml`
- Board: `esp32-c6-devkitc-1`
- Variant: `esp32c6`
- Framework: ESP-IDF
- Flash size: `8MB`
- CPU frequency: `160MHz`
- Sensor input pin: `GPIO0`
- Status LED pin: `GPIO8`
- OTA rollback: enabled

### WeMos D1 mini Pro Configuration

- Main file: `esphome/soil-moisture.yaml`
- Settings file: `esphome/settings_wemosd1.yaml`
- Device package: `esphome/packages/device_wemosd1.yaml`
- Board: `d1_mini_pro`
- Framework: Arduino / ESP8266
- Sensor input pin: `A0`
- Status LED pin: `GPIO2`
- ADC voltage scaling: `multiply: 3.3`

### Main Entities
- `Soil Moisture Percentage`
- `Soil Moisture Voltage`
- `WIFI Signal Strength`
- `WIFI SSID`
- `IP Address`
- `Firmware Version`
- `Software Version`
- `System Uptime`

The ESP32-C6 build also exposes:
- `WIFI Protocol`

### ESP32-C6 Status LED Behavior
The onboard WS2812 status LED is connected to `GPIO8` by default and uses color/effect states managed in `esphome/packages/device_esp32c6.yaml`.

- Blue pulse while booting
- Red pulse when Wi-Fi is disconnected
- Yellow pulse when appliances are configured but not connected
- White pulse when Wi-Fi is up but no API client is connected
- Purple pulse during normal connected operation

### WeMos D1 mini Pro Status LED Behavior
The WeMos build uses a single active-low `status_led` light on `GPIO2`. It provides simple built-in ESPHome status indication rather than the multi-color WS2812 behavior used on the ESP32-C6 build.

## Requirements

### Software
- ESPHome installed locally

### Hardware
- ESP32-C6-Zero, WeMos D1 mini Pro, or compatible board for the selected entry file
- Capacitive soil moisture sensor V2.0 with analog output
- USB cable for the initial flash

## Setup

### 1. Install ESPHome
Follow the official directions on the [ESPHome website](https://esphome.io/guides/installing_esphome/).

If you're using macOS, ESPHome can be installed with [Homebrew](https://brew.sh/):

```bash
brew install esphome
```

### 2. Clone the Repository

```bash
git clone https://github.com/katherine/esphome-soil-moisture.git
cd esphome-soil-moisture
```

### 3. Configure Secrets
Copy the example secrets file and add your own values:

```bash
cp esphome/examples/secrets-example.yaml esphome/secrets.yaml
```

Important values include:
- `wifi_ssid`
- `wifi_password`
- `ap_wifi_ssid`
- `ap_wifi_password`
- `web_server_username`
- `web_server_password`
- `ota_password`
- `encryption_key`

Generate a valid 32-byte API encryption key and paste it into `encryption_key`. ESPHome documents that process here: [ESPHome API encryption](https://esphome.io/components/api/).

### 4. Choose the Device Configuration
Use `esphome/soil-moisture.yaml` for both boards.

For the default ESP32-C6 configuration, leave the package includes as they are:

```yaml
packages:
  settings: !include settings.yaml
  helpers: !include packages/shared_helpers.yaml
  esp32c6: !include packages/device_esp32c6.yaml
```

For the WeMos D1 mini Pro, switch the includes to:

```yaml
packages:
  settings: !include settings_wemosd1.yaml
  helpers: !include packages/shared_helpers.yaml
  wemosd1: !include packages/device_wemosd1.yaml
```

### 5. Review Device Settings
Project-specific substitutions are defined in the device settings file:

- ESP32-C6-Zero: `esphome/settings.yaml`
- WeMos D1 mini Pro: `esphome/settings_wemosd1.yaml`

Common values you may want to change:
- `device_name`
- `friendly_name`
- `device_description`
- `update_interval`
- `sw_version`
- `sensor_gpio`
- `status_led_pin`
- `dry_voltage`
- `wet_voltage`

The current default calibration in this repository is:
- `dry_voltage: "2.4"`
- `wet_voltage: "1.0"`

If your sensor reports different voltages, update those values after measuring the probe in dry air and then in water or very wet soil.

The percentage sensor is calculated from the voltage sensor using this mapping:
- `x >= dry_voltage` becomes `0%`
- `x <= wet_voltage` becomes `100%`
- values between them are linearly interpolated

This means the current configuration expects higher voltage when the probe is dry and lower voltage when the probe is wet.

On the WeMos D1 mini Pro build, the ADC sensor is scaled with `multiply: 3.3` so the reported voltage matches the board's analog input divider behavior.

### 6. Validate the Configuration

```bash
esphome config esphome/soil-moisture.yaml
```

### 7. Build and Flash
For the first flash, connect the board over USB and run:

```bash
esphome run esphome/soil-moisture.yaml
```

After a successful build, ESPHome will prompt you to choose the upload target.

### 8. Subsequent Updates
After the initial USB flash, updates can be installed over the air:

```bash
esphome run esphome/soil-moisture.yaml
```

When prompted, choose the OTA device instead of the USB serial port.

### 9. Web UI
The built-in ESPHome web server starts on port `80`. Open the device on your local network and sign in with the username and password configured in `esphome/secrets.yaml`.

The web UI exposes the moisture, voltage, and diagnostic entities defined in the main YAML and helper package.

### 10. Home Assistant
Once the device is online, it should be discovered by the ESPHome integration in Home Assistant. Use the same `encryption_key` configured in `esphome/secrets.yaml` when adding it.

## Calibration Notes

- The `Soil Moisture Voltage` diagnostic sensor is the best way to tune calibration.
- The `Soil Moisture Percentage` entity is calculated directly from that voltage reading.
- A dry probe should read near `dry_voltage` and produce a moisture value near `0%`.
- A soaked probe should read near `wet_voltage` and produce a moisture value near `100%`.
- If the moisture percentage looks inverted or stuck near one extreme, update the two voltage thresholds in `esphome/settings.yaml`.
- If your measured dry voltage is lower than your measured wet voltage, the sensor wiring or hardware behavior differs from the default assumption and the thresholds should be adjusted accordingly.

## Wiring Notes

### ESP32-C6-Zero
- The soil moisture sensor analog output is connected to `GPIO0` by default.
- The status LED is connected to `GPIO8` by default.
- Both pins can be changed in `esphome/settings.yaml`.

### WeMos D1 mini Pro
- The soil moisture sensor analog output is connected to `A0` by default.
- The built-in status LED uses `GPIO2` by default.
- Both pins can be changed in `esphome/settings_wemosd1.yaml`, but `A0` is the only ADC input available on ESP8266 in ESPHome.
