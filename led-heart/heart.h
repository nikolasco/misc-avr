#include <inttypes.h>

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

#define DEBOUNCE_COUNTER TCNT0
#define DEBOUNCE_CONTROL TCCR0B

static const uint8_t DEBOUNCE_DELAY = 200;

static inline void start_debounce() {
  DEBOUNCE_COUNTER = 0;
  DEBOUNCE_CONTROL = _BV(CS02) | _BV(CS00);
}

static inline uint8_t debounce_passed() {
  if (DEBOUNCE_COUNTER <= DEBOUNCE_DELAY) return 0;

  DEBOUNCE_CONTROL = 0;
  DEBOUNCE_COUNTER = 0;

  return 1;
}

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

static inline void turn_led_on(uint8_t *ddr, uint8_t *port, uint8_t num) {
  set_output(ddr, LED_PINS[num].pos);
  set_output(ddr, LED_PINS[num].gnd);
  set_pos(port, LED_PINS[num].pos);
  set_gnd(port, LED_PINS[num].gnd);
}

