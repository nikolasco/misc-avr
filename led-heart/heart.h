// Copyright (c) 2010, Nikolas Coukouma. All rights reserved.
// Distributed under the terms of a BSD-style license. See COPYING for details.

#include <avr/io.h>
#include <inttypes.h>
#include "general-avr.h"

#define LED_DEF(pi_pos, pi_gnd) \
    { \
      pi_pos, pi_gnd \
    }
#define LED_PAIR(pi_pos, pi_gnd) \
    LED_DEF(pi_pos, pi_gnd), \
    LED_DEF(pi_gnd, pi_pos)

typedef struct {uint8_t pos, gnd;} led_pair_t;

static const led_pair_t
  LED_PINS[] = {
    LED_PAIR(5, 4),
    LED_PAIR(5, 3),
    LED_PAIR(5, 2),
    LED_PAIR(5, 1),
    LED_PAIR(5, 0),
    LED_PAIR(4, 3),
    LED_PAIR(4, 2),
    LED_PAIR(4, 1),
    LED_PAIR(4, 0),
    LED_PAIR(3, 2),
    LED_PAIR(3, 1),
    LED_PAIR(3, 0),
    LED_PAIR(2, 1),
    LED_DEF(2, 0),
  };
static const uint8_t NUM_LEDS = 27;

#define POWER_DDR DDRB
#define POWER_PORT PORTB
#define POWER_PIN PINB
static const uint8_t POWER_BIT = 2;

#define MODE_DDR DDRA
#define MODE_PORT PORTA
#define MODE_PIN PINA
static const uint8_t MODE_BIT = 6;

static inline void turn_led_on(uint8_t *ddr, uint8_t *port, uint8_t num) {
  set_output(ddr, LED_PINS[num].pos);
  set_output(ddr, LED_PINS[num].gnd);
  set_pos(port, LED_PINS[num].pos);
  set_gnd(port, LED_PINS[num].gnd);
}

static inline void turn_led_off(uint8_t *ddr, uint8_t *port, uint8_t num) {
  set_input(ddr, LED_PINS[num].pos);
  set_input(ddr, LED_PINS[num].gnd);
}


