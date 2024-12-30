#include "app_main.h"

#define DEBOUNCE_BUTTON     16                          /* number of polls for debounce       */
#define COUNT_FACTORY_RESET 5                           /* number of clicks for factory reset */

void init_button() {

    memset(&g_appCtx.button, 0, sizeof(button_t));

    g_appCtx.button.debounce = 1;
    g_appCtx.button.released_time = clock_time();

}

void button_handler() {

    if (!drv_gpio_read(BUTTON)) {
        if (g_appCtx.button.debounce != DEBOUNCE_BUTTON) {
            g_appCtx.button.debounce++;
            if (g_appCtx.button.debounce == DEBOUNCE_BUTTON) {
                g_appCtx.button.pressed = true;
                g_appCtx.button.pressed_time = clock_time();
                if (!clock_time_exceed(g_appCtx.button.released_time, TIMEOUT_TICK_1SEC)) {
                    g_appCtx.button.counter++;
                } else {
                    g_appCtx.button.counter = 1;
                }
            }
        }
    } else {
        if (g_appCtx.button.debounce != 1) {
            g_appCtx.button.debounce--;
            if (g_appCtx.button.debounce == 1 && g_appCtx.button.pressed) {
                g_appCtx.button.released = true;
                g_appCtx.button.released_time = clock_time();
            }
        }
    }

    if (g_appCtx.button.pressed && g_appCtx.button.released) {
        g_appCtx.button.pressed = g_appCtx.button.released = false;
        if (clock_time_exceed(g_appCtx.button.pressed_time, TIMEOUT_TICK_5SEC)) {
            /* long pressed > 5 sec. */
//            light_blink_start(3, 30, 250);
#if UART_PRINTF_MODE
            printf("Factory reset!\r\n");
#endif
            factory_reset = 1;
            analog_write(DEEP_ANA_REG1, factory_reset);

            zb_factoryReset();
        } else { /* short pressed < 5 sec. */
            light_blink_start(1, 100, 100);

            // for test
            //app_forcedReport(APP_ENDPOINT_1, ZCL_CLUSTER_GEN_DEVICE_TEMP_CONFIG, ZCL_ATTRID_DEV_TEMP_CURR_TEMP);
        }
    } else if (!g_appCtx.button.pressed) {
        if (clock_time_exceed(g_appCtx.button.released_time, TIMEOUT_TICK_1SEC)) {
            if (g_appCtx.button.counter == 1) {
                app_all_forceReporting();
            }
            g_appCtx.button.counter = 0;
        }
    }
}

uint8_t button_idle() {
    if ((g_appCtx.button.debounce != 1 && g_appCtx.button.debounce != DEBOUNCE_BUTTON)
            || g_appCtx.button.pressed
            || g_appCtx.button.counter) {
        return true;
    }
    return false;
}

