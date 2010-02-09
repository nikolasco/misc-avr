#include <avr/io.h>
#include <util/delay.h>
#include "heart.h"

// note that one second = F_CPU/1024 (currently 1000000/1024)
// also note that the maximum duration one frame = (2^16 - 1) * 1000000/1024
// or about 1min

typedef struct {uint8_t leds[27]; uint16_t dur;} anim_frame;
typedef struct {uint16_t num_frames; anim_frame frames[];} anim;

static const anim ON_OFF_ANIM =
  {2, {
    {{
           255, 255,      255, 255,
      255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255,
           255, 255, 255, 255, 255, 
                255, 255, 255,
                     255
      }, 900},
    {{
             0,   0,        0,   0,
        0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,
             0,   0,   0,   0,   0, 
                  0,   0,   0,
                       0
      }, 900},
  }};
static const anim UP_DOWN_ANIM =
  {3, {
    {{
             0,   0,        0,   0,
        0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,
             0,   0,   0,   0,   0, 
                  0,   0,   0,
                       0
      }, 325},
    {{
             0,   0,        0,   0,
        0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,
             0,   0,   0,   0,   0, 
                  0,   0,   0,
                     255
      }, 325},
    {{
             0,   0,        0,   0,
        0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,
             0,   0,   0,   0,   0, 
                255, 255, 255,
                     255
      }, 325},
    // including the rest of the frames overflows SRAM
    // TODO: move into flash
    /*
    {{
             0,   0,        0,   0,
        0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,
           255, 255, 255, 255, 255, 
                255, 255, 255,
                     255
      }, 325},
    {{
             0,   0,        0,   0,
        0,   0,   0,   0,   0,   0,   0,
      255, 255, 255, 255, 255, 255, 255,
           255, 255, 255, 255, 255, 
                255, 255, 255,
                     255
      }, 325},
    {{
             0,   0,        0,   0,
      255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255,
           255, 255, 255, 255, 255, 
                255, 255, 255,
                     255
      }, 325},
    {{
           255, 255,      255, 255,
      255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255,
           255, 255, 255, 255, 255, 
                255, 255, 255,
                     255
      }, 651},
    {{
             0,   0,        0,   0,
      255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255,
           255, 255, 255, 255, 255, 
                255, 255, 255,
                     255
      }, 325},
    {{
             0,   0,        0,   0,
        0,   0,   0,   0,   0,   0,   0,
      255, 255, 255, 255, 255, 255, 255,
           255, 255, 255, 255, 255, 
                255, 255, 255,
                     255
      }, 325},
    {{
             0,   0,        0,   0,
        0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,
           255, 255, 255, 255, 255, 
                255, 255, 255,
                     255
      }, 325},
    {{
             0,   0,        0,   0,
        0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,
             0,   0,   0,   0,   0, 
                255, 255, 255,
                     255
      }, 325},
    {{
             0,   0,        0,   0,
        0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,
             0,   0,   0,   0,   0, 
                  0,   0,   0,
                     255
      }, 325},
    {{
             0,   0,        0,   0,
        0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,
             0,   0,   0,   0,   0, 
                  0,   0,   0,
                       0
      }, 325},
    */
  }};
static const uint8_t NUM_ANIMATIONS = 2;
static const anim *ANIMATIONS[] = {&UP_DOWN_ANIM, &ON_OFF_ANIM};

int main() {
  DDRA = ALL_INPUT;
  PORTA = ALL_GND_PULL_OFF;
  DDRB = ALL_INPUT;
  PORTB = ALL_POS_PULL_ON;

  // set prescaler to 1024
  TCCR1B = _BV(CS12) | _BV(CS10);
  
  while (1) {
    for (uint8_t n = 0; n < NUM_ANIMATIONS; n++) {
      uint8_t next_animation = 0;
      while (1) {
        for (uint16_t f = 0; f < ANIMATIONS[n]->num_frames; f++) {
          for (TCNT1 = 0; TCNT1 < ANIMATIONS[n]->frames[f].dur; /*nothing*/) {
            for (uint8_t i = 0; i < 27; i++) {
              uint8_t tmp_port = ALL_GND_PULL_OFF, tmp_ddr = ALL_INPUT;
              if (ANIMATIONS[n]->frames[f].leds[i]) {
                turn_led_on(&tmp_ddr, &tmp_port, i);
              }
              DDRA = tmp_ddr;
              PORTA = tmp_port;
            }
            DDRA = ALL_INPUT;
            PORTA = ALL_POS_PULL_ON;
            DDRB = ALL_INPUT;
            PORTB = ALL_POS_PULL_ON;

            // TODO: make sure the button comes up
            // to make this less time-dependant
            if (bit_is_clear(MODE_PIN, MODE_BIT)) {
              next_animation = 1;
              _delay_ms(1000);
              break;
            }
          }
          if (next_animation) break;
        }
        if (next_animation) break;
      }
    }
  }
  DDRA = ALL_INPUT;
  PORTA = ALL_GND_PULL_OFF;
  return 0;
}
