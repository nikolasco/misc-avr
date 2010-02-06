#include <avr/io.h>
#include "heart.h"

int main() {
  while (1) {
    for(uint8_t i = 0; i < 27; i++) {
      static uint8_t tmp_port = 0, tmp_ddr = 0;
      tmp_ddr = LED_PINS[i].pos | LED_PINS[i].gnd;
      tmp_port = LED_PINS[i].pos;
      DDRA = tmp_ddr;
      PORTA = tmp_port;
    }
  }
  return 0;
}
