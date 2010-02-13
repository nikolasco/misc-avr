// Copyright (c) 2010, Nikolas Coukouma. All rights reserved.
// Distributed under the terms of a BSD-style license. See COPYING for details.

#include <avr/io.h>
#include "heart.h"
#include "dimming.h"

int main() {
  DDRB = ALL_INPUT;
  PORTB = ALL_POS_PULL_ON;
  DDRA = ALL_INPUT;
  PORTA = ALL_GND_PULL_OFF;
  while (1) {
    uint8_t dir = 0;
    for (uint8_t l = 0x0; 1; dir ? l++ : l--) {
      uint8_t v = lookup_lightness(l);
      for (uint8_t t = 0; t < BIT_SCHED_LEN; t++) {
        uint8_t on_now = lookup_on_off(v, t);
        for(uint8_t i = 0; i < NUM_LEDS; i++) {
          uint8_t tmp_port = ALL_GND_PULL_OFF, tmp_ddr = ALL_INPUT;
          if (on_now) turn_led_on(&tmp_ddr, &tmp_port, i);
          DDRA = tmp_ddr;
          PORTA = tmp_port;
        }
        DDRA = ALL_INPUT;
        PORTA = ALL_GND_PULL_OFF;
      }
      // switch directions instead of overflowing
      if (dir) {
        if (MAX_LIGHTNESS == l) dir = 0;
      } else {
        if (0 == l) dir = 1;
      }
    }
  }

  DDRA = ALL_INPUT;
  PORTA = ALL_GND_PULL_OFF;
  return 0;
}
