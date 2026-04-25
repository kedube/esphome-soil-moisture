# ESPHome Soil Moisture Sensor
ESPHome configuration for a capacitive soil moisture sensor running on an **ESP32-C6-Zero**.

This project exposes a calibrated soil moisture percentage, a diagnostic voltage reading, Wi-Fi diagnostics, and a simple local web UI. It is designed to be easy to flash, adjust, and integrate with Home Assistant.

## Repository Layout

- `esphome/soil-moisture.yaml` is the main ESPHome entry file.
- `esphome/settings.yaml` contains shared substitutions such as device name, update interval, GPIO selection, and moisture calibration voltages.
- `esphome/packages/device_esp32c6.yaml` contains the ESP32-C6-specific board settings and status LED behavior.
- `esphome/packages/shared_helpers.yaml` contains shared diagnostic sensors and text sensors.
- `esphome/examples/secrets-example.yaml` contains the template for the secrets file used by the main configuration.
- `include/status_led_helper.h` contains the status LED state logic.
- `include/wifi_protocol_helper.h` contains the Wi-Fi protocol helper used by the diagnostic text sensor.

## Features

- ESPHome integration for Home Assistant
- Local web interface with authentication
- OTA firmware updates
- Soil moisture percentage derived from sensor voltage
- Diagnostic voltage sensor for calibration
- Wi-Fi signal, SSID, IP address, uptime, and firmware version sensors
- ESP32-C6 status LED with state-based color/effect changes

## Requirements

### Software
- ESPHome installed locally

### Hardware
- ESP32-C6-Zero or compatible ESP32-C6 board
- Capacitive soil moisture sensor with analog output
- USB-C cable for the initial flash

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

### 4. Review Shared Settings
Project-specific substitutions are defined in `esphome/settings.yaml`.

Common values you may want to change:
- `device_name`
- `friendly_name`
- `update_interval`
- `sensor_gpio`
- `status_led_pin`
- `dry_voltage`
- `wet_voltage`

The default calibration assumes:
- `dry_voltage: "2.4"`
- `wet_voltage: "1.0"`

If your sensor reports different voltages, update those values after measuring the probe in dry air and then in water or very wet soil.

### 5. Validate the Configuration

```bash
esphome config esphome/soil-moisture.yaml
```

### 6. Build and Flash
For the first flash, connect the ESP32-C6 board over USB and run:

```bash
esphome run esphome/soil-moisture.yaml
```

After a successful build, ESPHome will prompt you to choose the upload target.

### 7. Subsequent Updates
After the initial USB flash, updates can be installed over the air:

```bash
esphome run esphome/soil-moisture.yaml
```

When prompted, choose the OTA device instead of the USB serial port.

### 8. Web UI
The built-in ESPHome web server starts on port `80`. Open the device on your local network and sign in with the username and password configured in `esphome/secrets.yaml`.

### 9. Home Assistant
Once the device is online, it should be discovered by the ESPHome integration in Home Assistant. Use the same `encryption_key` configured in `esphome/secrets.yaml` when adding it.

## Calibration Notes

- The diagnostic voltage sensor is the best way to tune calibration.
- A dry probe should read near `dry_voltage` and produce a moisture value near `0%`.
- A soaked probe should read near `wet_voltage` and produce a moisture value near `100%`.
- If the moisture percentage looks inverted or stuck near one extreme, update the two voltage thresholds in `esphome/settings.yaml`.

## Wiring Notes

- The soil moisture sensor analog output is connected to `GPIO0` by default.
- The status LED is connected to `GPIO8` by default.
- Both pins can be changed in `esphome/settings.yaml`.
