#include <avr/io.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <util/delay.h>
#include "heart.h"
#include "dimming.h"

// note that one second = F_CPU/1024 (currently 8000000/1024)
// also note that the maximum duration one frame = (2^16 - 1) * 1024/8000000
// or about 8s

typedef struct {
  const int8_t leds[27]; // on or off
  const uint16_t steady_dur;
  const uint16_t fade_dur;
} anim_frame;

typedef struct {
  const uint16_t num_frames;
  const anim_frame frames[];
} anim;

// Note: when adding new animations, remember to add them to the ANIMATIONS
// array below

static const PROGMEM anim ON_ANIM =
  {1, {
    {{
         1, 1,    1, 1,
      1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 1, 
            1, 1, 1,
               1
      }, 65000, 0}
  }};

static const PROGMEM anim PULSE_ANIM =
  {2, {
    {{
         0, 0,    0, 0,
      0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 
            0, 0, 0,
               0
      }, 3600, 8001},
    {{
         1, 1,    1, 1,
      1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 1, 
            1, 1, 1,
               1
      }, 3600, 8001},
  }};

static const PROGMEM anim UP_DOWN_ANIM =
  {12, {
    {{
         0, 0,    0, 0,
      0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 
            0, 0, 0,
               0
      }, 3900, 1280},
    {{
         0, 0,    0, 0,
      0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 
            0, 0, 0,
               1
      }, 0, 1280},
    {{
         0, 0,    0, 0,
      0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 
            1, 1, 1,
               1
      }, 0, 1280},
    {{
         0, 0,    0, 0,
      0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0,
         1, 1, 1, 1, 1, 
            1, 1, 1,
               1
      }, 0, 1280},
    {{
         0, 0,    0, 0,
      0, 0, 0, 0, 0, 0, 0,
      1, 1, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 1, 
            1, 1, 1,
               1
      }, 0, 1280},
    {{
         0, 0,    0, 0,
      1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 1, 
            1, 1, 1,
               1
      }, 0, 1280},
    {{
         1, 1,    1, 1,
      1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 1, 
            1, 1, 1,
               1
      }, 3900, 1280},
    {{
         0, 0,    0, 0,
      1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 1, 
            1, 1, 1,
               1
      }, 0, 1280},
    {{
         0, 0,    0, 0,
      0, 0, 0, 0, 0, 0, 0,
      1, 1, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 1, 
            1, 1, 1,
               1
      }, 0, 1280},
    {{
         0, 0,    0, 0,
      0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0,
         1, 1, 1, 1, 1, 
            1, 1, 1,
               1
      }, 0, 1280},
    {{
         0, 0,    0, 0,
      0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 
            1, 1, 1,
               1
      }, 0, 1280},
    {{
         0, 0,    0, 0,
      0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 
            0, 0, 0,
               1
      }, 0, 1280},
  }};

static const PROGMEM anim HEAL_ANIM =
  {6, {
    {{
         0, 0,    0, 0,
      0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 
            0, 0, 0,
               0
      }, 0, 1280},
    {{
         0, 0,    0, 0,
      0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 
            0, 0, 0,
               1
      }, 0, 1280},
    {{
         0, 0,    0, 0,
      0, 0, 0, 0, 0, 0, 0,
      1, 0, 0, 0, 0, 0, 1,
         1, 0, 0, 0, 1, 
            1, 0, 1,
               1
      }, 0, 1280},
    {{
         0, 0,    0, 0,
      1, 1, 0, 0, 0, 1, 1,
      1, 1, 0, 0, 0, 1, 1,
         1, 1, 0, 1, 1, 
            1, 1, 1,
               1
      }, 0, 1280},
    {{
         1, 0,    0, 1,
      1, 1, 0, 0, 0, 1, 1,
      1, 1, 1, 0, 1, 1, 1,
         1, 1, 0, 1, 1, 
            1, 1, 1,
               1
      }, 0, 1280},
    {{
         1, 1,    1, 1,
      1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 1, 
            1, 1, 1,
               1
      }, 0, 12800},
  }};

static const PROGMEM anim BREAK_ANIM =
  {7, {
    {{
         1, 1,    1, 1,
      1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 1, 
            1, 1, 1,
               1
      }, 0, 1280},
    {{
         1, 1,    1, 1,
      1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 0, 1, 1, 1,
         1, 1, 1, 1, 1, 
            1, 0, 1,
               1
      }, 0, 1280},
    {{
         1, 1,    1, 1,
      1, 1, 1, 0, 1, 1, 1,
      1, 1, 0, 0, 1, 1, 1,
         1, 1, 0, 1, 1, 
            1, 0, 0,
               0
      }, 0, 1280},
    {{
         1, 0,    1, 1,
      1, 1, 0, 0, 1, 1, 1,
      1, 1, 0, 0, 1, 1, 1,
         1, 1, 0, 0, 0, 
            1, 0, 0,
               0
      }, 0, 1280},
    {{
         0, 0,    1, 1,
      0, 0, 0, 0, 0, 1, 1,
      1, 1, 0, 0, 0, 0, 1,
         1, 1, 0, 0, 0, 
            1, 0, 0,
               0
      }, 0, 1280},
    {{
         0, 0,    0, 1,
      0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 
            1, 0, 0,
               0
      }, 0, 1280},
    {{
         0, 0,    0, 0,
      0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 
            0, 0, 0,
               0
      }, 0, 12800},
  }};

static const PROGMEM anim THUMP_THUMP_ANIM =
  {12, {
    {{
         1, 1,    1, 1,
      1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 1, 
            1, 1, 1,
               1
      }, 6500, 640},
    {{
         0, 0,    0, 0,
      0, 1, 1, 0, 1, 1, 0,
      0, 1, 1, 1, 1, 1, 0,
         0, 1, 1, 1, 0, 
            0, 1, 0,
               0
      }, 0, 640},
    {{
         0, 0,    0, 0,
      0, 0, 0, 0, 0, 0, 0,
      0, 0, 1, 1, 1, 0, 0,
         0, 0, 1, 0, 0, 
            0, 0, 0,
               0
      }, 0, 640},
    {{
         0, 0,    0, 0,
      0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 
            0, 0, 0,
               0
      }, 0, 640},
    {{
         0, 0,    0, 0,
      0, 0, 0, 0, 0, 0, 0,
      0, 0, 1, 1, 1, 0, 0,
         0, 0, 1, 0, 0, 
            0, 0, 0,
               0
      }, 0, 640},
    {{
         0, 0,    0, 0,
      0, 1, 1, 0, 1, 1, 0,
      0, 1, 1, 1, 1, 1, 0,
         0, 1, 1, 1, 0, 
            0, 1, 0,
               0
      }, 0, 640},
    {{
         1, 1,    1, 1,
      1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 1, 
            1, 1, 1,
               1
      }, 0, 640},
    {{
         0, 0,    0, 0,
      0, 1, 1, 0, 1, 1, 0,
      0, 1, 1, 1, 1, 1, 0,
         0, 1, 1, 1, 0, 
            0, 1, 0,
               0
      }, 0, 640},
    {{
         0, 0,    0, 0,
      0, 0, 0, 0, 0, 0, 0,
      0, 0, 1, 1, 1, 0, 0,
         0, 0, 1, 0, 0, 
            0, 0, 0,
               0
      }, 0, 640},
    {{
         0, 0,    0, 0,
      0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 
            0, 0, 0,
               0
      }, 0, 640},
    {{
         0, 0,    0, 0,
      0, 0, 0, 0, 0, 0, 0,
      0, 0, 1, 1, 1, 0, 0,
         0, 0, 1, 0, 0, 
            0, 0, 0,
               0
      }, 0, 640},
    {{
         0, 0,    0, 0,
      0, 1, 1, 0, 1, 1, 0,
      0, 1, 1, 1, 1, 1, 0,
         0, 1, 1, 1, 0, 
            0, 1, 0,
               0
      }, 0, 640},
  }};

static const anim *ANIMATIONS[] = {
  &ON_ANIM, &THUMP_THUMP_ANIM, &UP_DOWN_ANIM, &PULSE_ANIM, &HEAL_ANIM, &BREAK_ANIM};

// determine number of animations automatically
static const uint8_t NUM_ANIMATIONS = sizeof(ANIMATIONS)/sizeof(ANIMATIONS[0]);

uint8_t next_animation = 0;

static void shine_leds(uint16_t dur, uint8_t cur_vals[]) {
  for (TCNT1 = 0; TCNT1 < dur; /*nothing*/) {
    for (uint8_t t = 0; t < BIT_SCHED_LEN; t++) {
      for (uint8_t i = 0; i < NUM_LEDS; i++) {
        uint8_t tmp_port = ALL_GND_PULL_OFF, tmp_ddr = ALL_INPUT;
        if (lookup_on_off(cur_vals[i], t)) {
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
      
      if (TCNT1 > dur) break;
    }

    if (next_animation) break;
  }
}

int main() {
  DDRA = ALL_INPUT;
  PORTA = ALL_GND_PULL_OFF;
  DDRB = ALL_INPUT;
  PORTB = ALL_POS_PULL_ON;

  // set prescaler to 1024
  TCCR1B = _BV(CS12) | _BV(CS10);
  
  while (1) {
    for (uint8_t n = 0; n < NUM_ANIMATIONS; n++) {
      next_animation = 0;
      uint16_t frame_count = pgm_read_dword(&ANIMATIONS[n]->num_frames);
      while (1) {
        for (uint16_t f = 0; f < frame_count; f++) {
          uint8_t cur_vals[NUM_LEDS];
          anim_frame cur_frame;
          int8_t cur_lights[NUM_LEDS], next_frame_diff[NUM_LEDS];
          uint16_t fade_step;

          memcpy_P(&cur_frame, &ANIMATIONS[n]->frames[f], sizeof(cur_frame));
          memcpy(&cur_lights, cur_frame.leds, sizeof(cur_lights));
          memcpy_P(&next_frame_diff,
                   &ANIMATIONS[n]->frames[(f+1 < frame_count) ? f+1 : 0].leds,
                   sizeof(next_frame_diff));

          for (uint8_t i = 0; i < NUM_LEDS; i++) {
            next_frame_diff[i] -= cur_frame.leds[i];

            cur_lights[i] = cur_frame.leds[i]? 127 : 0;
            cur_vals[i] = lookup_lightness(cur_lights[i]);
          }
          fade_step = cur_frame.fade_dur / MAX_LIGHTNESS;

          shine_leds(cur_frame.steady_dur, cur_vals);

          if (next_animation) break;

          for (uint8_t s = 0; s < MAX_LIGHTNESS; s++) {
            for (uint8_t i = 0; i < NUM_LEDS; i++) {
              cur_lights[i] += next_frame_diff[i];
              cur_vals[i] = lookup_lightness(cur_lights[i]);
            }

            shine_leds(fade_step, cur_vals);

            if (next_animation) break;
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
