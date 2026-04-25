#pragma once

#include <string>

#include "esp_err.h"
#include "esp_wifi.h"

inline std::string get_negotiated_wifi_protocol() {
  wifi_phy_mode_t phy_mode;
  const esp_err_t err = esp_wifi_sta_get_negotiated_phymode(&phy_mode);

  if (err != ESP_OK) {
    return "Disconnected";
  }

  // ESP-IDF documents that bit 5 may be used to indicate LR support in the
  // negotiated mode. Mask it off before decoding the base enum value.
  const uint8_t raw_mode = static_cast<uint8_t>(phy_mode);
  const bool lr_enabled = (raw_mode & 0x20U) != 0;
  const wifi_phy_mode_t base_mode = static_cast<wifi_phy_mode_t>(raw_mode & 0x1FU);

  switch (base_mode) {
    case WIFI_PHY_MODE_LR:
      return "802.11 LR";
    case WIFI_PHY_MODE_11B:
      return lr_enabled ? "802.11b + LR" : "802.11b";
    case WIFI_PHY_MODE_11G:
      return lr_enabled ? "802.11g + LR" : "802.11g";
    case WIFI_PHY_MODE_11A:
      return "802.11a";
    case WIFI_PHY_MODE_HT20:
    case WIFI_PHY_MODE_HT40:
      return lr_enabled ? "802.11n + LR" : "802.11n";
    case WIFI_PHY_MODE_HE20:
      return "802.11ax";
    case WIFI_PHY_MODE_VHT20:
      return "802.11ac";
    default:
      return "Unknown";
  }
}
