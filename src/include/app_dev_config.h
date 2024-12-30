#ifndef SRC_INCLUDE_APP_DEV_CONFIG_H_
#define SRC_INCLUDE_APP_DEV_CONFIG_H_

#define CFG_NUMBER_OF_DIGITS    5
#define CFG_DIVISOR             1
#define CFG_SERIAL_NUMBER       {8,'1','1','1','1','1','1','1','1'}
#define CFG_DEVICE_NAME         {7,'U','n','k','n','o','w','n'}

/* must be no more than FLASH_PAGE_SIZE (256) bytes */
typedef struct __attribute__((packed)) {
    uint32_t id;                    /* ID - ID_CONFIG                   */
    uint8_t  new_ota;               /* new ota flag                     */
    uint32_t top;                   /* 0x0 .. 0xFFFFFFFF                */
    uint32_t flash_addr_start;      /* flash page address start         */
    uint32_t flash_addr_end;        /* flash page address end           */
    uint32_t summ_delivered;        /* currentSummDelivered             */
    uint32_t energy_counter;        /* Last number of energy counter    */
    uint16_t energy_divisor;        /* example 3200 imp/kWh             */
    uint8_t  serial_number[ZCL_BASIC_MAX_LENGTH + 1];
    uint8_t  device_name[ZCL_BASIC_MAX_LENGTH + 1];
    uint8_t  number_digits;
    uint16_t crc;
} dev_config_t;

extern dev_config_t dev_config;

void init_config(uint8_t print);
void write_config();

#endif /* SRC_INCLUDE_APP_DEV_CONFIG_H_ */
