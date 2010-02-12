// Copyright (c) 2010, Nikolas Coukouma. All rights reserved.
// Distributed under the terms of a BSD-style license. See COPYING for details.

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "heart.h"

static const uint32_t DEBOUNCE_POWER_LOOPS = F_CPU/150;

EMPTY_INTERRUPT(PCINT1_vect);

int main() {
  DDRB = ALL_INPUT;
  PORTB = ALL_POS_PULL_ON;
  DDRA = ALL_INPUT;
  PORTA = ALL_GND_PULL_OFF;
  cli();
  while (1) {
    for(uint8_t i = 0; i < 27; i++) {
      uint8_t tmp_port = ALL_GND_PULL_OFF, tmp_ddr = ALL_INPUT;
      turn_led_on(&tmp_ddr, &tmp_port, i);
      DDRA = tmp_ddr;
      PORTA = tmp_port;
    }
    DDRA = ALL_INPUT;
    PORTA = ALL_GND_PULL_OFF;
    if (bit_is_clear(POWER_PIN, POWER_BIT)) {
      // we really just want to go into power down mode, and be woken up when
      // someone hits the "power" button
      _delay_ms(1000);
      cli();
      set_sleep_mode(SLEEP_MODE_PWR_DOWN);
      sleep_enable();
      GIMSK |= _BV(PCIE1);
      PCMSK1 |= _BV(PCINT10);
      sei();

      sleep_cpu();

      cli();
      sleep_disable();
      GIMSK &= ~_BV(INT0) & ~_BV(PCIE1);
      PCMSK1 &= ~_BV(PCINT10);
      MCUCR &= ~_BV(ISC01);
      _delay_ms(500);
    }
  }

  DDRA = ALL_INPUT;
  PORTA = ALL_GND_PULL_OFF;
  return 0;
}
