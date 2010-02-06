#include <inttypes.h>

#define LED_DEF(pi_pos, pi_gnd) \
    { \
      _BV(pi_pos), _BV(pi_gnd) \
    }
#define LED_PAIR(pi_pos, pi_gnd) \
    LED_DEF(pi_pos, pi_gnd), \
    LED_DEF(pi_gnd, pi_pos)

static const struct {uint8_t pos, gnd;}
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
