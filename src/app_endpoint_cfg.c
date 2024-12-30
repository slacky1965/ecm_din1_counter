#include "tl_common.h"
#include "zcl_include.h"

#include "app_main.h"

#ifndef ZCL_BASIC_MFG_NAME
#define ZCL_BASIC_MFG_NAME          {6,'T','E','L','I','N','K'}
#endif
#ifndef ZCL_BASIC_MODEL_ID
#define ZCL_BASIC_MODEL_ID          {8,'T','L','S','R','8','2','x','x'}
#endif
#ifndef ZCL_BASIC_SW_BUILD_ID
#define ZCL_BASIC_SW_BUILD_ID       {10,'0','1','2','2','0','5','2','0','1','7'}
#endif


#define R               ACCESS_CONTROL_READ
#define RW              ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE
#define RR              ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE
#define RRW             ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE | ACCESS_CONTROL_WRITE

#define ZCL_INT16       ZCL_DATA_TYPE_INT16
#define ZCL_UINT8       ZCL_DATA_TYPE_UINT8
#define ZCL_UINT16      ZCL_DATA_TYPE_UINT16
#define ZCL_UINT24      ZCL_DATA_TYPE_UINT24
#define ZCL_UINT32      ZCL_DATA_TYPE_UINT32
#define ZCL_UINT48      ZCL_DATA_TYPE_UINT48
#define ZCL_UINT64      ZCL_DATA_TYPE_UINT64
#define ZCL_ENUM8       ZCL_DATA_TYPE_ENUM8
#define ZCL_ENUM16      ZCL_DATA_TYPE_ENUM16
#define ZCL_BITMAP8     ZCL_DATA_TYPE_BITMAP8
#define ZCL_BITMAP16    ZCL_DATA_TYPE_BITMAP16
#define ZCL_BOOLEAN     ZCL_DATA_TYPE_BOOLEAN
#define ZCL_CHAR_STR    ZCL_DATA_TYPE_CHAR_STR
#define ZCL_OCTET_STR   ZCL_DATA_TYPE_OCTET_STR
#define ZCL_UTC         ZCL_DATA_TYPE_UTC
#define ZCL_IEEE_ADDR   ZCL_DATA_TYPE_IEEE_ADDR

/**
 *  @brief Definition for Incoming cluster / Sever Cluster
 */
const uint16_t app_inClusterList[] =
{
    ZCL_CLUSTER_GEN_BASIC,
    ZCL_CLUSTER_GEN_IDENTIFY,
#ifdef ZCL_POLL_CTRL
    ZCL_CLUSTER_GEN_POLL_CONTROL,
#endif
    ZCL_CLUSTER_SE_METERING,
    ZCL_CLUSTER_GEN_DEVICE_TEMP_CONFIG,
};

/**
 *  @brief Definition for Outgoing cluster / Client Cluster
 */
const uint16_t app_outClusterList[] =
{
#ifdef ZCL_GROUP
    ZCL_CLUSTER_GEN_GROUPS,
#endif
#ifdef ZCL_SCENE
    ZCL_CLUSTER_GEN_SCENES,
#endif
#ifdef ZCL_LEVEL_CTRL
    ZCL_CLUSTER_GEN_LEVEL_CONTROL,
#endif
#ifdef ZCL_OTA
    ZCL_CLUSTER_OTA,
#endif
#ifdef ZCL_ZLL_COMMISSIONING
    ZCL_CLUSTER_TOUCHLINK_COMMISSIONING,
#endif
    ZCL_CLUSTER_GEN_TIME,
    ZCL_CLUSTER_SE_METERING,
};

/**
 *  @brief Definition for Server cluster number and Client cluster number
 */
#define APP_IN_CLUSTER_NUM   (sizeof(app_inClusterList)/sizeof(app_inClusterList[0]))
#define APP_OUT_CLUSTER_NUM  (sizeof(app_outClusterList)/sizeof(app_outClusterList[0]))

/**
 *  @brief Definition for simple description for HA profile
 */
const af_simple_descriptor_t app_simpleDesc = {
    HA_PROFILE_ID,                          /* Application profile identifier */
    HA_DEV_METER_INTERFACE,                 /* Application device identifier */
    APP_ENDPOINT1,                          /* Endpoint */
    2,                                      /* Application device version */
    0,                                      /* Reserved */
    APP_IN_CLUSTER_NUM,                     /* Application input cluster count */
    APP_OUT_CLUSTER_NUM,                    /* Application output cluster count */
    (uint16_t *)app_inClusterList,          /* Application input cluster list */
    (uint16_t *)app_outClusterList,         /* Application output cluster list */
};


/* Basic */
zcl_basicAttr_t g_zcl_basicAttrs =
{
    .zclVersion     = 0x03,
    .appVersion     = APP_RELEASE,
    .stackVersion   = (STACK_RELEASE|STACK_BUILD),
    .hwVersion      = HW_VERSION,
    .manuName       = ZCL_BASIC_MFG_NAME,
    .modelId        = ZCL_BASIC_MODEL_ID,
    .dateCode       = ZCL_BASIC_DATE_CODE,
    .powerSource    = POWER_SOURCE_MAINS_1_PHASE,
    .swBuildId      = ZCL_BASIC_SW_BUILD_ID,
    .deviceEnable   = TRUE,
};

const zclAttrInfo_t basic_attrTbl[] =
{
    { ZCL_ATTRID_BASIC_ZCL_VER,             ZCL_UINT8,      R,  (uint8_t*)&g_zcl_basicAttrs.zclVersion      },
    { ZCL_ATTRID_BASIC_APP_VER,             ZCL_UINT8,      R,  (uint8_t*)&g_zcl_basicAttrs.appVersion      },
    { ZCL_ATTRID_BASIC_STACK_VER,           ZCL_UINT8,      R,  (uint8_t*)&g_zcl_basicAttrs.stackVersion    },
    { ZCL_ATTRID_BASIC_HW_VER,              ZCL_UINT8,      R,  (uint8_t*)&g_zcl_basicAttrs.hwVersion       },
    { ZCL_ATTRID_BASIC_MFR_NAME,            ZCL_CHAR_STR,   R,  (uint8_t*)g_zcl_basicAttrs.manuName         },
    { ZCL_ATTRID_BASIC_MODEL_ID,            ZCL_CHAR_STR,   R,  (uint8_t*)g_zcl_basicAttrs.modelId          },
    { ZCL_ATTRID_BASIC_DATE_CODE,           ZCL_CHAR_STR,   R,  (uint8_t*)g_zcl_basicAttrs.dateCode         },
    { ZCL_ATTRID_BASIC_POWER_SOURCE,        ZCL_ENUM8,      R,  (uint8_t*)&g_zcl_basicAttrs.powerSource     },
    { ZCL_ATTRID_BASIC_DEV_ENABLED,         ZCL_BOOLEAN,    RW, (uint8_t*)&g_zcl_basicAttrs.deviceEnable    },
    { ZCL_ATTRID_BASIC_SW_BUILD_ID,         ZCL_CHAR_STR,   R,  (uint8_t*)&g_zcl_basicAttrs.swBuildId       },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,     R,  (uint8_t*)&zcl_attr_global_clusterRevision  },

};

#define ZCL_BASIC_ATTR_NUM       sizeof(basic_attrTbl) / sizeof(zclAttrInfo_t)


/* Identify */
zcl_identifyAttr_t g_zcl_identifyAttrs =
{
    .identifyTime   = 0x0000,
};

const zclAttrInfo_t identify_attrTbl[] =
{
    { ZCL_ATTRID_IDENTIFY_TIME,             ZCL_UINT16,     RW, (uint8_t*)&g_zcl_identifyAttrs.identifyTime },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,     R,  (uint8_t*)&zcl_attr_global_clusterRevision  },
};

#define ZCL_IDENTIFY_ATTR_NUM           sizeof(identify_attrTbl) / sizeof(zclAttrInfo_t)


#ifdef ZCL_POLL_CTRL
/* Poll Control */
zcl_pollCtrlAttr_t g_zcl_pollCtrlAttrs =
{
    .chkInInterval          = 0x3840,
    .longPollInterval       = 0x14,
    .shortPollInterval      = 0x02,
    .fastPollTimeout        = 0x28,
    .chkInIntervalMin       = 0x00,
    .longPollIntervalMin    = 0x00,
    .fastPollTimeoutMax     = 0x00,
};

const zclAttrInfo_t pollCtrl_attrTbl[] =
{
    { ZCL_ATTRID_CHK_IN_INTERVAL,           ZCL_UINT32, RW, (uint8_t*)&g_zcl_pollCtrlAttrs.chkInInterval        },
    { ZCL_ATTRID_LONG_POLL_INTERVAL,        ZCL_UINT32, R,  (uint8_t*)&g_zcl_pollCtrlAttrs.longPollInterval     },
    { ZCL_ATTRID_SHORT_POLL_INTERVAL,       ZCL_UINT16, R,  (uint8_t*)&g_zcl_pollCtrlAttrs.shortPollInterval    },
    { ZCL_ATTRID_FAST_POLL_TIMEOUT,         ZCL_UINT16, RW, (uint8_t*)&g_zcl_pollCtrlAttrs.fastPollTimeout      },
    { ZCL_ATTRID_CHK_IN_INTERVAL_MIN,       ZCL_UINT32, R,  (uint8_t*)&g_zcl_pollCtrlAttrs.chkInIntervalMin     },
    { ZCL_ATTRID_LONG_POLL_INTERVAL_MIN,    ZCL_UINT32, R,  (uint8_t*)&g_zcl_pollCtrlAttrs.longPollIntervalMin  },
    { ZCL_ATTRID_FAST_POLL_TIMEOUT_MAX,     ZCL_UINT16, R,  (uint8_t*)&g_zcl_pollCtrlAttrs.fastPollTimeoutMax   },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16, R,  (uint8_t*)&zcl_attr_global_clusterRevision          },
};

#define ZCL_POLLCTRL_ATTR_NUM           sizeof(pollCtrl_attrTbl) / sizeof(zclAttrInfo_t)
#endif


zcl_timeAttr_t g_zcl_timeAttrs = {
    .time_utc = 0xffffffff,
};

const zclAttrInfo_t time_attrTbl[] =
{
    { ZCL_ATTRID_TIME,                      ZCL_UTC,    RW, (uint8_t*)&g_zcl_timeAttrs.time_utc         },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16, R,  (uint8_t*)&zcl_attr_global_clusterRevision  },
};

#define ZCL_TIME_ATTR_NUM    sizeof(time_attrTbl) / sizeof(zclAttrInfo_t)


zcl_tempAttr_t g_zcl_tempAttrs = {
        .temperature    = 22,
        .alarm_mask     = 0x02, /* bit0 = 0 (low alarm is disabled), bit1 = 1 (high alarm is enabled) */
        .high_threshold = 70,   /* 70 in degrees Celsius                                              */
};

const zclAttrInfo_t temp_attrTbl[] =
{
    {ZCL_ATTRID_DEV_TEMP_CURR_TEMP,         ZCL_INT16,      RR, (uint8_t*)&g_zcl_tempAttrs.temperature      },
    {ZCL_ATTRID_DEV_TEMP_ALARM_MASK,        ZCL_BITMAP8,    RW, (uint8_t*)&g_zcl_tempAttrs.alarm_mask       },
    {ZCL_ATTRID_DEV_TEMP_HIGH_THRES,        ZCL_INT16,      RW, (uint8_t*)&g_zcl_tempAttrs.high_threshold   },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,     R,  (uint8_t*)&zcl_attr_global_clusterRevision  },
};

#define ZCL_TEMP_ATTR_NUM    sizeof(temp_attrTbl) / sizeof(zclAttrInfo_t)


zcl_seAttr_t g_zcl_seAttrs = {
    .status = 0,
    .unit_of_measure = 0x00,        // 0x00 - kWh
    .summation_formatting = 0xFA,   // bit7 - 1, bit6-bit3 - 15, bit2-bit0 - 2 (b11111010)
    .deviceType = 0,                // 0 - Electric Metering
    .multiplier = 1,
    .divisor = 100,
    .cur_sum_delivered = 0,
    .energy_preset = 0,
    .divisor_preset = 100,
    .serial_number = CFG_SERIAL_NUMBER,
    .device_name = CFG_DEVICE_NAME,
    .user_serial_number = CFG_SERIAL_NUMBER,
    .number_digits = 5,
};

/* Attribute record list */
const zclAttrInfo_t zcl_se_attrTbl[] = {
    { ZCL_ATTRID_CURRENT_SUMMATION_DELIVERD,    ZCL_UINT48,     RR,     (uint8_t*)&g_zcl_seAttrs.cur_sum_delivered      },
    { ZCL_ATTRID_STATUS,                        ZCL_BITMAP8,    R,      (uint8_t*)&g_zcl_seAttrs.status                 },
    { ZCL_ATTRID_UNIT_OF_MEASURE,               ZCL_ENUM8,      R,      (uint8_t*)&g_zcl_seAttrs.unit_of_measure        },
    { ZCL_ATTRID_MULTIPLIER,                    ZCL_UINT24,     R,      (uint8_t*)&g_zcl_seAttrs.multiplier             },
    { ZCL_ATTRID_DIVISOR,                       ZCL_UINT24,     R,      (uint8_t*)&g_zcl_seAttrs.divisor                },
    { ZCL_ATTRID_SUMMATION_FORMATTING,          ZCL_BITMAP8,    R,      (uint8_t*)&g_zcl_seAttrs.summation_formatting   },
    { ZCL_ATTRID_METERING_DEVICE_TYPE,          ZCL_BITMAP8,    R,      (uint8_t*)&g_zcl_seAttrs.deviceType             },
    { ZCL_ATTRID_METER_SERIAL_NUMBER,           ZCL_OCTET_STR,  RR,     (uint8_t*)g_zcl_seAttrs.serial_number           },
    { ZCL_ATTRID_CUSTOM_ENERGY_PRESET,          ZCL_UINT48,     RW,     (uint8_t*)&g_zcl_seAttrs.energy_preset          },
    { ZCL_ATTRID_CUSTOM_DIVISOR_PRESET,         ZCL_UINT16,     RRW,    (uint8_t*)&g_zcl_seAttrs.divisor_preset         },
    { ZCL_ATTRID_CUSTOM_NUMBER_DIGITS,          ZCL_ENUM8,      RRW,    (uint8_t*)&g_zcl_seAttrs.number_digits          },
    { ZCL_ATTRID_CUSTOM_SERIAL_NUMBER,          ZCL_OCTET_STR,  RW,     (uint8_t*)g_zcl_seAttrs.serial_number           },
    { ZCL_ATTRID_CUSTOM_DEVICE_NAME,            ZCL_OCTET_STR,  RRW,    (uint8_t*)g_zcl_seAttrs.device_name             },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,       ZCL_UINT16,     R,  (uint8_t*)&zcl_attr_global_clusterRevision      },
};

#define ZCL_SE_ATTR_NUM         sizeof(zcl_se_attrTbl) / sizeof(zclAttrInfo_t)


/**
 *  @brief Definition for simple switch ZCL specific cluster
 */
const zcl_specClusterInfo_t g_appClusterList[] =
{
    {ZCL_CLUSTER_GEN_BASIC,     MANUFACTURER_CODE_NONE, ZCL_BASIC_ATTR_NUM,     basic_attrTbl,      zcl_basic_register,     app_basicCb     },
    {ZCL_CLUSTER_GEN_IDENTIFY,  MANUFACTURER_CODE_NONE, ZCL_IDENTIFY_ATTR_NUM,  identify_attrTbl,   zcl_identify_register,  app_identifyCb  },
#ifdef ZCL_GROUP
    {ZCL_CLUSTER_GEN_GROUPS,        MANUFACTURER_CODE_NONE, 0,                  NULL,               zcl_group_register,     app_groupCb     },
#endif
#ifdef ZCL_SCENE
    {ZCL_CLUSTER_GEN_SCENES,        MANUFACTURER_CODE_NONE, 0,                  NULL,               zcl_scene_register,     app_sceneCb     },
#endif
    {ZCL_CLUSTER_GEN_TIME,      MANUFACTURER_CODE_NONE, ZCL_TIME_ATTR_NUM,      time_attrTbl,       zcl_time_register,      app_timeCb      },
    {ZCL_CLUSTER_SE_METERING,   MANUFACTURER_CODE_NONE, ZCL_SE_ATTR_NUM,        zcl_se_attrTbl,     zcl_metering_register,  app_meteringCb  },
    {ZCL_CLUSTER_GEN_DEVICE_TEMP_CONFIG, MANUFACTURER_CODE_NONE, ZCL_TEMP_ATTR_NUM, temp_attrTbl,   zcl_devTemperatureCfg_register, NULL    },
};

uint8_t APP_CB_CLUSTER_NUM = (sizeof(g_appClusterList)/sizeof(g_appClusterList[0]));

