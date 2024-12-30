#include "tl_common.h"
#include "zcl_include.h"

#include "app_main.h"

#define ID_CONFIG           0x0FED1410
#define TOP_MASK            0xFFFFFFFF

dev_config_t dev_config;

static uint8_t  default_config = false;
static uint32_t config_addr_start = 0;
static uint32_t config_addr_end = 0;

uint8_t mcuBootAddrGet(void);

static uint16_t checksum(const uint8_t *src_buffer, uint8_t len) {

    const uint16_t generator = 0xa010;

    uint16_t crc = 0xffff;

    len -= 2;

    for (const uint8_t *ptr = src_buffer; ptr < src_buffer + len; ptr++) {
        crc ^= *ptr;

        for (uint8_t bit = 8; bit > 0; bit--) {
            if (crc & 1)
                crc = (crc >> 1) ^ generator;
            else
                crc >>= 1;
        }
    }

    return crc;
}

static void get_user_data_addr(uint8_t print) {
#ifdef ZCL_OTA
    if (mcuBootAddrGet()) {
        config_addr_start = BEGIN_USER_DATA1;
        config_addr_end = END_USER_DATA1;
#if UART_PRINTF_MODE
        if (print) printf("OTA mode enabled. MCU boot from address: 0x%x\r\n", BEGIN_USER_DATA2);
#endif /* UART_PRINTF_MODE */
    } else {
        config_addr_start = BEGIN_USER_DATA2;
        config_addr_end = END_USER_DATA2;
#if UART_PRINTF_MODE
        if (print) printf("OTA mode enabled. MCU boot from address: 0x%x\r\n", BEGIN_USER_DATA1);
#endif /* UART_PRINTF_MODE */
    }
#else
    config_addr_start = BEGIN_USER_DATA2;
    config_addr_end = END_USER_DATA2;

#if UART_PRINTF_MODE
    if (print) printf("OTA mode desabled. MCU boot from address: 0x%x\r\n", BEGIN_USER_DATA1);
#endif /* UART_PRINTF_MODE */

#endif
}

static void set_attr_from_config() {

    uint64_t energy          = dev_config.summ_delivered;
    uint16_t divisor         = dev_config.energy_divisor * 100;
    uint8_t number_of_digits = dev_config.number_digits;

    zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SE_METERING, ZCL_ATTRID_METER_SERIAL_NUMBER, (uint8_t*)dev_config.serial_number);
    zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SE_METERING, ZCL_ATTRID_CUSTOM_DEVICE_NAME, (uint8_t*)dev_config.device_name);
    zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SE_METERING, ZCL_ATTRID_CUSTOM_NUMBER_DIGITS, (uint8_t*)&number_of_digits);
    zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SE_METERING, ZCL_ATTRID_CUSTOM_DIVISOR_PRESET, (uint8_t*)&divisor);
    zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SE_METERING, ZCL_ATTRID_CURRENT_SUMMATION_DELIVERD, (uint8_t*)&energy);
}

static void clear_user_data(uint32_t flash_addr) {

    uint32_t flash_data_size = flash_addr + USER_DATA_SIZE;

    while(flash_addr < flash_data_size) {
        flash_erase_sector(flash_addr);
        flash_addr += FLASH_SECTOR_SIZE;
    }
}

static void init_default_config() {
    uint8_t serial_number[] = CFG_SERIAL_NUMBER;
    uint8_t device_name[] = CFG_DEVICE_NAME;
    memset(&dev_config, 0, sizeof(dev_config_t));
    dev_config.id = ID_CONFIG;
    dev_config.top = 0;
    dev_config.new_ota = 0;
    dev_config.energy_counter = 0;
    dev_config.energy_divisor = CFG_DIVISOR;
    dev_config.flash_addr_start = config_addr_start;
    dev_config.flash_addr_end = config_addr_end;
    dev_config.number_digits = CFG_NUMBER_OF_DIGITS;
    memcpy(dev_config.serial_number, serial_number, sizeof(serial_number));
    memcpy(dev_config.device_name, device_name, sizeof(device_name));
    default_config = true;
    write_config();
    set_attr_from_config();
}

static void write_restore_config() {
    dev_config.crc = checksum((uint8_t*)&(dev_config), sizeof(dev_config_t));

#if UART_PRINTF_MODE && DEBUG_CONFIG
    nv_sts_t st = nv_flashWriteNew(1, NV_MODULE_APP,  NV_ITEM_APP_USER_CFG, sizeof(dev_config_t), (uint8_t*)&dev_config);
    printf("Save restored config to nv_ram in module NV_MODULE_APP (%d) item NV_ITEM_APP_USER_CFG (%d), return: %d\r\n",
            NV_MODULE_APP,  NV_ITEM_APP_USER_CFG, st);
#else
    nv_flashWriteNew(1, NV_MODULE_APP,  NV_ITEM_APP_USER_CFG, sizeof(dev_config_t), (uint8_t*)&dev_config);
#endif /* UART_PRINTF_MODE */

}

void init_config(uint8_t print) {
    dev_config_t config_curr, config_next, config_restore;
    uint8_t find_config = false;
    nv_sts_t st = NV_SUCC;

    get_user_data_addr(print);

#if !NV_ENABLE
#error "NV_ENABLE must be enable in "stack_cfg.h" file!"
#endif

    st = nv_flashReadNew(1, NV_MODULE_APP,  NV_ITEM_APP_USER_CFG, sizeof(dev_config_t), (uint8_t*)&config_restore);
    //flash_read(GEN_USER_CFG_DATA, sizeof(dev_config_t), (uint8_t*)&config_restore);

    uint16_t crc = checksum((uint8_t*)&config_restore, sizeof(dev_config_t));

    if (st != NV_SUCC || config_restore.id != ID_CONFIG || crc != config_restore.crc) {
#if UART_PRINTF_MODE && DEBUG_CONFIG
        printf("No saved config! Init.\r\n");
#endif /* UART_PRINTF_MODE */
        clear_user_data(config_addr_start);
        init_default_config();
        return;

    }

    if (config_restore.new_ota) {
        config_restore.new_ota = false;
        config_restore.flash_addr_start = config_addr_start;
        config_restore.flash_addr_end = config_addr_end;
        memcpy(&dev_config, &config_restore, sizeof(dev_config_t));
        default_config = true;
        write_config();
        set_attr_from_config();
        return;
    }

    uint32_t flash_addr = config_addr_start;

    flash_read_page(flash_addr, sizeof(dev_config_t), (uint8_t*)&config_curr);

    if (config_curr.id != ID_CONFIG) {
#if UART_PRINTF_MODE && DEBUG_CONFIG
        printf("No saved config! Init.\r\n");
#endif /* UART_PRINTF_MODE */
        clear_user_data(config_addr_start);
        init_default_config();
        return;
    }

    flash_addr += FLASH_PAGE_SIZE;

    while(flash_addr < config_addr_end) {
        flash_read_page(flash_addr, sizeof(dev_config_t), (uint8_t*)&config_next);
        crc = checksum((uint8_t*)&config_next, sizeof(dev_config_t));
        if (config_next.id == ID_CONFIG && crc == config_next.crc) {
            if ((config_curr.top + 1) == config_next.top || (config_curr.top == TOP_MASK && config_next.top == 0)) {
                memcpy(&config_curr, &config_next, sizeof(dev_config_t));
                flash_addr += FLASH_PAGE_SIZE;
                continue;
            }
            find_config = true;
            break;
        }
        find_config = true;
        break;
    }

    if (find_config) {
        memcpy(&dev_config, &config_curr, sizeof(dev_config_t));
        dev_config.flash_addr_start = flash_addr-FLASH_PAGE_SIZE;
#if UART_PRINTF_MODE && DEBUG_CONFIG
        printf("Read config from flash address - 0x%x\r\n", dev_config.flash_addr_start);
#endif /* UART_PRINTF_MODE */
        set_attr_from_config();
    } else {
#if UART_PRINTF_MODE && DEBUG_CONFIG
        printf("No active saved config! Reinit.\r\n");
#endif /* UART_PRINTF_MODE */
        clear_user_data(config_addr_start);
        init_default_config();
    }
}

void write_config() {
    if (default_config) {
        write_restore_config();
        flash_erase(dev_config.flash_addr_start);
        flash_write(dev_config.flash_addr_start, sizeof(dev_config_t), (uint8_t*)&(dev_config));
        default_config = false;
#if UART_PRINTF_MODE && DEBUG_CONFIG
        printf("Save config to flash address - 0x%x\r\n", dev_config.flash_addr_start);
#endif /* UART_PRINTF_MODE */
    } else {
        if (!dev_config.new_ota) {
            dev_config.flash_addr_start += FLASH_PAGE_SIZE;
            if (dev_config.flash_addr_start == config_addr_end) {
                dev_config.flash_addr_start = config_addr_start;
            }
            if (dev_config.flash_addr_start % FLASH_SECTOR_SIZE == 0) {
                flash_erase(dev_config.flash_addr_start);
            }
            dev_config.top++;
            dev_config.top &= TOP_MASK;
            dev_config.crc = checksum((uint8_t*)&(dev_config), sizeof(dev_config_t));
            flash_write(dev_config.flash_addr_start, sizeof(dev_config_t), (uint8_t*)&(dev_config));
#if UART_PRINTF_MODE && DEBUG_CONFIG
            printf("Save config to flash address - 0x%x\r\n", dev_config.flash_addr_start);
#endif /* UART_PRINTF_MODE */
        } else {
            write_restore_config();
        }
    }

}

