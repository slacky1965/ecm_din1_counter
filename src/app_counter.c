#include "tl_common.h"
#include "zcl_include.h"

#include "app_main.h"

#define DEBOUNCE_COUNTER    8                          /* number of polls for debounce       */

static energy_counter_t energy_counter;

uint32_t check_counter_overflow(uint32_t check_count) {
    uint32_t count;
    uint32_t counter_overflow;

    if (dev_config.number_digits == 5) {
        counter_overflow = OVERFLOW_DIGITS_5;
    } else if (dev_config.number_digits == 6) {
        counter_overflow = OVERFLOW_DIGITS_6;
    } else {
        counter_overflow = OVERFLOW_DIGITS_5;
    }

    if (check_count >= counter_overflow) {
        count = check_count - counter_overflow;
#if UART_PRINTF_MODE && DEBUG_COUNTER
        printf("Counter overflow. Nember of digits: %d. Old value: %d, new value: %d\r\n", counter_overflow, check_count, count);
#endif /* UART_PRINTF_MODE */
    } else {
        count = check_count;
    }

    return count;
}

/* reg_deep_sleep is                                                */
/* 0x01: input level on HOT_PIN before deep sleep                   */
/* 0x02: input level on COLD_PIN before deep sleep                  */
/* 0x04: 1 - deep sleep, watchdog or soft_reset,  0 - new start MCU */

void init_counter() {

    energy_counter.counter = 0;
    energy_counter.debounce = 1;
}

uint8_t counter_handler() {

    uint8_t save_config = false;
    uint64_t enc_counter = 0;

#if 0
    gpio_setup_up_down_resistor(HOT_GPIO, PM_PIN_PULLUP_10K);
    gpio_setup_up_down_resistor(COLD_GPIO, PM_PIN_PULLUP_10K);
#endif

    if (!drv_gpio_read(ENC_GPIO)) {
        if (energy_counter.debounce != DEBOUNCE_COUNTER) {
            energy_counter.debounce++;
            if (energy_counter.debounce == DEBOUNCE_COUNTER) {
                energy_counter.counter++;
            }
        }
    } else {
        if (energy_counter.debounce != 1) {
            energy_counter.debounce--;
        }
    }


    if (energy_counter.counter) {

        dev_config.energy_counter += energy_counter.counter;
        energy_counter.counter = 0;

        light_blink_stop();
        light_blink_start(1, 250, 250);

#if UART_PRINTF_MODE && DEBUG_COUNTER
        printf("energy_counter - %d\r\n", dev_config.energy_counter);
#endif /* UART_PRINTF_MODE */

        if (dev_config.energy_counter / dev_config.energy_divisor == 1 &&
            dev_config.energy_counter % dev_config.energy_divisor == 0) {
            dev_config.summ_delivered++;
            /* detect counter overflow */
            dev_config.summ_delivered = check_counter_overflow(dev_config.summ_delivered);
            dev_config.energy_counter = 0;
#if UART_PRINTF_MODE && DEBUG_COUNTER
            printf("all energy - %d\r\n", dev_config.summ_delivered);
#endif /* UART_PRINTF_MODE */
            enc_counter = dev_config.summ_delivered & 0xffffffffffff;
            zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SE_METERING, ZCL_ATTRID_CURRENT_SUMMATION_DELIVERD, (uint8_t*)&enc_counter);
        }

//        energy_counter.counter = 0;
//#if UART_PRINTF_MODE && DEBUG_COUNTER
//        printf("energy_counter - %d\r\n", dev_config.energy_counter);
//#endif /* UART_PRINTF_MODE */
//        enc_counter = dev_config.energy_counter & 0xffffffffffff;
//        zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SE_METERING, ZCL_ATTRID_CURRENT_SUMMATION_DELIVERD, (uint8_t*)&enc_counter);
        write_config();
    }


#if 0
    gpio_setup_up_down_resistor(HOT_GPIO, PM_PIN_PULLUP_1M);
    gpio_setup_up_down_resistor(COLD_GPIO, PM_PIN_PULLUP_1M);
#endif

    if (counter_idle()) {
        sleep_ms(1);
    }

    return save_config;
}

uint8_t counter_idle() {
    if (energy_counter.debounce != 1 && energy_counter.debounce != DEBOUNCE_COUNTER) {
        return true;
    }
    return false;
}

void reset_counter() {
    init_counter();
    dev_config.energy_counter = 0;
    write_config();
}
