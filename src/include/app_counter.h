#ifndef SRC_INCLUDE_APP_COUNTER_H_
#define SRC_INCLUDE_APP_COUNTER_H_

#define OVERFLOW_DIGITS_5       10000000        /* counters overflow 5 digits + 2   */
#define OVERFLOW_DIGITS_6       100000000       /* counters overflow 6 digits + 2   */

typedef struct {
    uint8_t  counter;
    uint8_t  debounce;
} energy_counter_t;

void init_counter();
uint8_t counter_handler();
uint32_t check_counter_overflow(uint32_t check_count);
uint8_t counter_idle();
void reset_counter();

#endif /* SRC_INCLUDE_APP_COUNTER_H_ */
