// Copyright (c) 2010, Nikolas Coukouma. All rights reserved.
// Distributed under the terms of a BSD-style license. See COPYING for details.

#include <avr/io.h>
#include <inttypes.h>

static const uint8_t ALL_INPUT = 0, ALL_OUTPUT = ~0,
  ALL_POS_PULL_ON = ~0, ALL_GND_PULL_OFF = 0;

static inline void set_output(uint8_t *ddr, uint8_t pin_no) {
  *ddr |= _BV(pin_no);
}

static inline void set_input(uint8_t *ddr, uint8_t pin_no) {
  *ddr &= ~_BV(pin_no);
}

static inline void set_pos(uint8_t *port, uint8_t pin_no) {
  set_output(port, pin_no);
}

static inline void set_gnd(uint8_t *port, uint8_t pin_no) {
  set_input(port, pin_no);
}

static inline void set_pull_on(uint8_t *port, uint8_t pin_no) {
  set_output(port, pin_no);
}

static inline void set_pull_off(uint8_t *port, uint8_t pin_no) {
  set_input(port, pin_no);
}
