#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "gp.h"

#include "app_main.h"

int32_t flashLedStatusCb(void *arg) {
    if (!zb_isDeviceJoinedNwk() || !device_online) {
        light_blink_stop();
        light_blink_start(1, 250, 250);
    }

    return 0;
}

void led_on(uint32_t pin){
    drv_gpio_write(pin, LED_ON);
}

void led_off(uint32_t pin){
    drv_gpio_write(pin, LED_OFF);
}

void led_init(void){
    led_on(LED_STATUS);
}

void light_on(void)
{
    led_on(LED_STATUS);
}

void light_off(void)
{
    led_off(LED_STATUS);
}

void light_init(void)
{
    led_init();
}

int32_t zclLightTimerCb(void *arg)
{
    uint32_t interval = 0;

    if(g_appCtx.sta == g_appCtx.oriSta){
        g_appCtx.times--;
        if(g_appCtx.times <= 0){
            g_appCtx.timerLedEvt = NULL;
            return -1;
        }
    }

    g_appCtx.sta = !g_appCtx.sta;
    if(g_appCtx.sta){
        light_off();
        interval = g_appCtx.ledOnTime;
    }else{
        light_on();
        interval = g_appCtx.ledOffTime;
    }

    return interval;
}

void light_blink_start(uint8_t times, uint16_t ledOnTime, uint16_t ledOffTime)
{
    uint32_t interval = 0;
    g_appCtx.times = times;

    if(!g_appCtx.timerLedEvt){
        if(g_appCtx.oriSta){
            light_on();
            g_appCtx.sta = 0;
            interval = ledOffTime;
        }else{
            light_off();
            g_appCtx.sta = 1;
            interval = ledOnTime;
        }
        g_appCtx.ledOnTime = ledOnTime;
        g_appCtx.ledOffTime = ledOffTime;

        g_appCtx.timerLedEvt = TL_ZB_TIMER_SCHEDULE(zclLightTimerCb, NULL, interval);
    }
}

void light_blink_stop(void)
{
    if(g_appCtx.timerLedEvt){
        TL_ZB_TIMER_CANCEL(&g_appCtx.timerLedEvt);

        g_appCtx.times = 0;
        if(g_appCtx.oriSta){
            light_off();
        }else{
        }
    }
}

