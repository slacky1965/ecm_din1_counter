#ifndef SRC_INCLUDE_APP_UTILITY_H_
#define SRC_INCLUDE_APP_UTILITY_H_

/* for clock_time_exceed() */
#define TIMEOUT_TICK_50MS   50*1000         /* timeout 50 ms    */
#define TIMEOUT_TICK_100MS  100*1000        /* timeout 100 ms   */
#define TIMEOUT_TICK_200MS  200*1000        /* timeout 200 ms   */
#define TIMEOUT_TICK_250MS  250*1000        /* timeout 250 ms   */
#define TIMEOUT_TICK_1SEC   1000*1000       /* timeout 1 sec    */
#define TIMEOUT_TICK_2SEC   2*1000*1000     /* timeout 2 sec    */
#define TIMEOUT_TICK_2_5SEC 25*100*1000     /* timeout 2.5 sec  */
#define TIMEOUT_TICK_3SEC   3*1000*1000     /* timeout 3 sec    */
#define TIMEOUT_TICK_5SEC   5*1000*1000     /* timeout 5 sec    */
#define TIMEOUT_TICK_10SEC  10*1000*1000    /* timeout 10 sec   */
#define TIMEOUT_TICK_15SEC  15*1000*1000    /* timeout 15 sec   */
#define TIMEOUT_TICK_30SEC  30*1000*1000    /* timeout 30 sec   */

/* for TL_ZB_TIMER_SCHEDULE() */
#define TIMEOUT_100MS              100      /* timeout 100 ms   */
#define TIMEOUT_500MS              500      /* timeout 900 ms   */
#define TIMEOUT_900MS              900      /* timeout 900 ms   */
#define TIMEOUT_1SEC        1    * 1000     /* timeout 1 sec    */
#define TIMEOUT_1200MS      12   * 100      /* timeout 1.2 sec  */
#define TIMEOUT_2SEC        2    * 1000     /* timeout 2 sec    */
#define TIMEOUT_3SEC        3    * 1000     /* timeout 3 sec    */
#define TIMEOUT_4SEC        4    * 1000     /* timeout 4 sec    */
#define TIMEOUT_5SEC        5    * 1000     /* timeout 5 sec    */
#define TIMEOUT_10SEC       10   * 1000     /* timeout 10 sec   */
#define TIMEOUT_15SEC       15   * 1000     /* timeout 15 sec   */
#define TIMEOUT_30SEC       30   * 1000     /* timeout 30 sec   */
#define TIMEOUT_1MIN        60   * 1000     /* timeout 1 min    */
#define TIMEOUT_2MIN        120  * 1000     /* timeout 2 min    */
#define TIMEOUT_5MIN        300  * 1000     /* timeout 5 min    */
#define TIMEOUT_10MIN       600  * 1000     /* timeout 10 min   */
#define TIMEOUT_15MIN       900  * 1000     /* timeout 15 min   */
#define TIMEOUT_30MIN       1800 * 1000     /* timeout 30 min   */
#define TIMEOUT_60MIN       3600 * 1000     /* timeout 60 min   */

#define BUILD_U48(b0, b1, b2, b3, b4, b5)   ( (uint64_t)((((uint64_t)(b5) & 0x0000000000ff) << 40) + (((uint64_t)(b4) & 0x0000000000ff) << 32) + (((uint64_t)(b3) & 0x0000000000ff) << 24) + (((uint64_t)(b2) & 0x0000000000ff) << 16) + (((uint64_t)(b1) & 0x0000000000ff) << 8) + ((uint64_t)(b0) & 0x00000000FF)) )

void start_message();
int32_t poll_rateAppCb(void *arg);
int32_t delayedMcuResetCb(void *arg);
int32_t delayedFactoryResetCb(void *arg);
int32_t delayedFullResetCb(void *arg);
uint32_t reverse32(uint32_t in);
uint16_t reverse16(uint16_t in);
int32_t int32_from_str(uint8_t *data);
int16_t int16_from_str(uint8_t *data);
char * mystrstr(char * mainStr, char * subStr);
uint8_t set_zcl_str(uint8_t *str_in, uint8_t *str_out, uint8_t len);
uint32_t itoa(uint32_t value, uint8_t *ptr);
uint8_t *print_str_zcl(uint8_t *str_zcl);

#endif /* SRC_INCLUDE_APP_UTILITY_H_ */
