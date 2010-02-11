#include <avr/pgmspace.h>
#include <inttypes.h>

/*
 * Given a desired 7-bit lightness level, this maps it to a 8-bit luminance
 * value. Note that this consumes 128 bytes of flash memory.
 */
static const PROGMEM uint8_t LIGHTNESS_VAL[] = {
  0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5, 5, 6, 6, 6, 7, 7,
  8, 8, 9, 10, 10, 11, 11, 12, 13, 14, 14, 15, 16, 17, 18, 18, 19, 20, 21, 22,
  23, 24, 25, 27, 28, 29, 30, 31, 33, 34, 35, 37, 38, 40, 41, 43, 44, 46, 48,
  50, 51, 53, 55, 57, 59, 61, 63, 65, 67, 69, 71, 74, 76, 78, 81, 83, 85, 88,
  91, 93, 96, 99, 101, 104, 107, 110, 113, 116, 119, 122, 126, 129, 132, 135,
  139, 142, 146, 150, 153, 157, 161, 165, 169, 172, 177, 181, 185, 189, 193,
  198, 202, 207, 211, 216, 220, 225, 230, 235, 240, 245, 250, 255
};

static const uint8_t MAX_LIGHTNESS = sizeof(LIGHTNESS_VAL)/sizeof(LIGHTNESS_VAL[0]) - 1;

static inline uint8_t lookup_lightness(uint8_t l) {
  return pgm_read_dword(&LIGHTNESS_VAL[l]);
}

// this consumes 255 bytes of flash memory
static const PROGMEM uint8_t BIT_SCHED[] = {
  7, 6, 7, 5, 7, 6, 7, 4, 7, 6, 7, 5, 7, 6, 7, 3, 7, 6, 7, 5, 7, 6, 7, 4, 7, 6,
  7, 5, 7, 6, 7, 2, 7, 6, 7, 5, 7, 6, 7, 4, 7, 6, 7, 5, 7, 6, 7, 3, 7, 6, 7, 5,
  7, 6, 7, 4, 7, 6, 7, 5, 7, 6, 7, 1, 7, 6, 7, 5, 7, 6, 7, 4, 7, 6, 7, 5, 7, 6,
  7, 3, 7, 6, 7, 5, 7, 6, 7, 4, 7, 6, 7, 5, 7, 6, 7, 2, 7, 6, 7, 5, 7, 6, 7, 4,
  7, 6, 7, 5, 7, 6, 7, 3, 7, 6, 7, 5, 7, 6, 7, 4, 7, 6, 7, 5, 7, 6, 7, 0, 7, 6,
  7, 5, 7, 6, 7, 4, 7, 6, 7, 5, 7, 6, 7, 3, 7, 6, 7, 5, 7, 6, 7, 4, 7, 6, 7, 5,
  7, 6, 7, 2, 7, 6, 7, 5, 7, 6, 7, 4, 7, 6, 7, 5, 7, 6, 7, 3, 7, 6, 7, 5, 7, 6,
  7, 4, 7, 6, 7, 5, 7, 6, 7, 1, 7, 6, 7, 5, 7, 6, 7, 4, 7, 6, 7, 5, 7, 6, 7, 3,
  7, 6, 7, 5, 7, 6, 7, 4, 7, 6, 7, 5, 7, 6, 7, 2, 7, 6, 7, 5, 7, 6, 7, 4, 7, 6,
  7, 5, 7, 6, 7, 3, 7, 6, 7, 5, 7, 6, 7, 4, 7, 6, 7, 5, 7, 6, 7
};

static const uint16_t BIT_SCHED_LEN = sizeof(BIT_SCHED)/sizeof(BIT_SCHED[0]);

/*
 * given a time (t), lookup what bit to look at to determine whether to turn
 * the LED on or off.
 */
static inline uint8_t lookup_bit(uint8_t t) {
  return pgm_read_byte(&BIT_SCHED[t]);
}

// returns 0 if LED with value v should be off at time t and non-zero otherwise
static inline uint8_t lookup_on_off(uint8_t v, uint8_t t) {
  return v & _BV(lookup_bit(t));
}
