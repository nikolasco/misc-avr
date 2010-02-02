#include <util/delay.h>
#include <avr/io.h>
#include <inttypes.h>

static const uint8_t BUTTON_PIN = 5;

static inline void turn_on_led(volatile uint8_t *port, uint8_t pin) {
  *port |= _BV(pin);
}

static inline void turn_off_led(volatile uint8_t *port, uint8_t pin) {
  *port &= ~(_BV(pin));
}

static const double DELAY = 250;

int main() {
  DDRB = 0xff;
  PORTB = 0;
  DDRD = 0xff;
  PORTD = 0;

  while(1) {
    int8_t i;
    for(i = 0; i < 8; i++) {
      turn_on_led(&PORTB, i);
      _delay_ms(DELAY);
    }
    for(i = 0; i < 8; i++) {
      turn_on_led(&PORTD, i);
      _delay_ms(DELAY);
    }
    for(i = 7; i >= 0; i--) {
      turn_off_led(&PORTD, i);
      _delay_ms(DELAY);
    }
    for(i = 7; i >= 0; i--) {
      turn_off_led(&PORTB, i);
      _delay_ms(DELAY);
    }
  }
  
  return 0;
}
