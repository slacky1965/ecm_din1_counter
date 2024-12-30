#ifndef SRC_INCLUDE_APP_TIME_H_
#define SRC_INCLUDE_APP_TIME_H_

#define UNIX_TIME_CONST 946684800

void set_time_sent();
int32_t app_time_cmdCb(void *arg);
bool get_time_sent();

#endif /* SRC_INCLUDE_APP_TIME_H_ */
