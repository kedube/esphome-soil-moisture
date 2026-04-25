#pragma once

#include "esphome/components/light/light_call.h"
#include "esphome/components/light/light_state.h"

enum StatusLedState : int {
  STATUS_LED_OFF = 0,
  STATUS_LED_RED = 1,
  STATUS_LED_BLUE = 2,
  STATUS_LED_YELLOW = 3,
  STATUS_LED_WHITE = 4,
  STATUS_LED_PURPLE = 5,
};

inline int compute_status_led_state(bool booting, bool wifi_connected, bool api_connected,
                                    int connected_appliances, int configured_appliances) {
  if (booting) {
    return STATUS_LED_BLUE;
  }
  if (!wifi_connected) {
    return STATUS_LED_RED;
  }
  if (configured_appliances > 0 && connected_appliances <= 0) {
    return STATUS_LED_YELLOW;
  }
  if (!api_connected) {
    return STATUS_LED_WHITE;
  }
  return STATUS_LED_PURPLE;
}

inline void apply_status_led(esphome::light::LightState *status_led, int next_state) {
  if (next_state == STATUS_LED_OFF) {
    auto call = status_led->turn_off();
    call.perform();
    return;
  }

  auto call = status_led->turn_on();
  call.set_brightness(1.0f);

  switch (next_state) {
    case STATUS_LED_RED:
      call.set_rgb(1.0f, 0.0f, 0.0f);
      call.set_effect("Pulse Red");
      break;
    case STATUS_LED_BLUE:
      call.set_rgb(0.0f, 0.0f, 1.0f);
      call.set_effect("Pulse Blue");
      break;
    case STATUS_LED_YELLOW:
      call.set_rgb(1.0f, 1.0f, 0.0f);
      call.set_effect("Pulse Yellow");
      break;
    case STATUS_LED_WHITE:
      call.set_rgb(1.0f, 1.0f, 1.0f);
      call.set_effect("Pulse White");
      break;
    default:
      call.set_rgb(1.0f, 0.0f, 1.0f);
      call.set_effect("Pulse Purple");
      break;
  }

  call.perform();
}
