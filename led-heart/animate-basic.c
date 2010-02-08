#include <avr/io.h>
#include "heart.h"

// note that one second = F_CPU/1024 (currently 1000000/1024)
// also note that the maximum duration one frame = (2^16 - 1) * 1000000/1024
// or about 1min

typedef struct {uint8_t leds[27]; uint16_t dur;} anim_frame;
typedef struct {uint16_t num_frames; anim_frame frames[];} anim;

static const anim ANIMATION = {2, {
  {{
         255, 255,      255, 255,
    255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255,
         255, 255, 255, 255, 255, 
              255, 255, 255,
                   255
    }, 976},
  {{
           0,   0,        0,   0,
      0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,
           0,   0,   0,   0,   0, 
                0,   0,   0,
                     0
    }, 976}
}};

int main() {
  DDRA = ALL_INPUT;
  PORTA = ALL_GND_PULL_OFF;
  DDRB = ALL_INPUT;
  PORTB = ALL_POS_PULL_ON;

  // set prescaler to 1024
  TCCR1B |= _BV(CS12) | _BV(CS10);
  
  while (1) {
    uint16_t f = 0;
    for (uint16_t f = 0; f < ANIMATION.num_frames; f++) {
      for (TCNT1 = 0; TCNT1 < ANIMATION.frames[f].dur; /*nothing*/) {
        for (uint8_t i = 0; i < 27; i++) {
          uint8_t tmp_port = ALL_GND_PULL_OFF, tmp_ddr = ALL_INPUT;
          if (ANIMATION.frames[f].leds[i]) {
            turn_led_on(&tmp_ddr, &tmp_port, i);
          }
          DDRA = tmp_ddr;
          PORTA = tmp_port;
        }
      }
    }
  }
  DDRA = ALL_INPUT;
  PORTA = ALL_GND_PULL_OFF;
  return 0;
}
