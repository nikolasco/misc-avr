#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "heart.h"

// note that one second = F_CPU/1024 (currently 1000000/1024)
// also note that the maximum duration one frame = (2^16 - 1) * 1000000/1024
// or about 1min

typedef struct {const uint8_t leds[27]; const uint16_t dur;} anim_frame;
typedef struct {const uint16_t num_frames; const anim_frame frames[];} anim;

static const PROGMEM anim ON_OFF_ANIM =
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
static const PROGMEM anim UP_DOWN_ANIM =
  {13, {
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
      uint16_t frame_count = 0;
      memcpy_P(&frame_count, &ANIMATIONS[n]->num_frames, sizeof(frame_count));
      while (1) {
        for (uint16_t f = 0; f < frame_count; f++) {
          anim_frame cur_frame;
          memcpy_P(&cur_frame, &ANIMATIONS[n]->frames[f], sizeof(cur_frame));
          for (TCNT1 = 0; TCNT1 < cur_frame.dur; /*nothing*/) {
            for (uint8_t i = 0; i < 27; i++) {
              uint8_t tmp_port = ALL_GND_PULL_OFF, tmp_ddr = ALL_INPUT;
              if (cur_frame.leds[i]) {
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
