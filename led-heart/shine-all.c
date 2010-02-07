#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "heart.h"

static const uint32_t DEBOUNCE_POWER_LOOPS = F_CPU/150;

//EMPTY_INTERRUPT(EXT_INT0_vect);
ISR(EXT_INT0_vect){}

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
      _delay_ms(100);
      if (bit_is_clear(POWER_PIN, POWER_BIT)) {
        // we really just want to go into power down mode, and be woken up when
        // someone hits the "power" button
        _delay_ms(500);
        // this pin change interrupt business is inspired by the Game of Life's
        // super wacky code. INT0 should work for us, right???
        PCMSK1 |= _BV(PCINT10);
        MCUCR |= _BV(ISC01);
        GIMSK |= _BV(INT0) | _BV(PCIE1);
        sleep_enable();
        //set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        set_sleep_mode(SLEEP_MODE_IDLE);
        sei();
        sleep_cpu();
        sleep_disable();
        cli();
        _delay_ms(500);
      }
    }
  }

  DDRA = ALL_INPUT;
  PORTA = ALL_GND_PULL_OFF;
  return 0;
}
