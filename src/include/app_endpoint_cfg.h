#ifndef SRC_INCLUDE_APP_ENDPOINT_CFG_H_
#define SRC_INCLUDE_APP_ENDPOINT_CFG_H_

#define APP_ENDPOINT1 0x01

/**
 *  @brief Defined for basic cluster attributes
 */
typedef struct{
    uint8_t  zclVersion;
    uint8_t  appVersion;
    uint8_t  stackVersion;
    uint8_t  hwVersion;
    uint8_t  manuName[ZCL_BASIC_MAX_LENGTH];
    uint8_t  modelId[ZCL_BASIC_MAX_LENGTH];
    uint8_t  dateCode[ZCL_BASIC_MAX_LENGTH];
    uint8_t  powerSource;
    uint8_t  genDevClass;                        //attr 8
    uint8_t  genDevType;                         //attr 9
    uint8_t  deviceEnable;
    uint8_t  swBuildId[ZCL_BASIC_MAX_LENGTH];    //attr 4000
} zcl_basicAttr_t;

/**
 *  @brief Defined for identify cluster attributes
 */
typedef struct{
    uint16_t identifyTime;
}zcl_identifyAttr_t;

typedef struct {
    uint8_t     status;
    uint8_t     unit_of_measure;        // 0x00 - kWh
    uint32_t    multiplier;             // 1
    uint32_t    divisor;                // 100
    uint8_t     summation_formatting;   // Bits 0 to 2: Number of Digits to the right of the Decimal Point
                                        // Bits 3 to 6: Number of Digits to the left of the Decimal Point
                                        // Bit  7:      If set, suppress leading zeros
    uint8_t     deviceType;             // 0 - Electric Metering
    uint64_t    cur_sum_delivered;
    uint64_t    energy_preset;
    uint16_t    divisor_preset;
    uint8_t     number_digits;
    uint8_t     serial_number[ZCL_BASIC_MAX_LENGTH + 1];
    uint8_t     device_name[ZCL_BASIC_MAX_LENGTH + 1];
    uint8_t     user_serial_number[ZCL_BASIC_MAX_LENGTH + 1];
} zcl_seAttr_t;

/**
 *  @brief  Defined for poll control cluster attributes
 */
typedef struct {
    uint32_t chkInInterval;
    uint32_t longPollInterval;
    uint32_t chkInIntervalMin;
    uint32_t longPollIntervalMin;
    uint16_t shortPollInterval;
    uint16_t fastPollTimeout;
    uint16_t fastPollTimeoutMax;
} zcl_pollCtrlAttr_t;

typedef struct {
    int16_t temperature;
    uint8_t alarm_mask;         /* bit0 = 0 (low alarm is disabled), bit1 = 1 (high alarm is enabled) */
    int16_t high_threshold;     /* 70 in degrees Celsius                                              */
} zcl_tempAttr_t;

typedef struct {
    uint32_t time_utc;
} zcl_timeAttr_t;


extern uint8_t APP_CB_CLUSTER_NUM;
extern const zcl_specClusterInfo_t g_appClusterList[];
extern const af_simple_descriptor_t app_simpleDesc;


/* Attributes */
extern zcl_basicAttr_t      g_zcl_basicAttrs;
extern zcl_identifyAttr_t   g_zcl_identifyAttrs;
extern zcl_pollCtrlAttr_t   g_zcl_pollCtrlAttrs;
extern zcl_seAttr_t         g_zcl_seAttrs;

#define zcl_iasZoneAttrGet()    &g_zcl_iasZoneAttrs
#define zcl_pollCtrlAttrGet()   &g_zcl_pollCtrlAttrs
#define zcl_seAttrs()           &g_zcl_seAttrs


void app_zclProcessIncomingMsg(zclIncoming_t *pInHdlrMsg);

void app_leaveCnfHandler(nlme_leave_cnf_t *pLeaveCnf);
void app_leaveIndHandler(nlme_leave_ind_t *pLeaveInd);
void app_otaProcessMsgHandler(uint8_t evt, uint8_t status);
bool app_nwkUpdateIndicateHandler(nwkCmd_nwkUpdate_t *pNwkUpdate);

status_t app_basicCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_identifyCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_powerCfgCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_groupCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_sceneCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_pollCtrlCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_meteringCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_timeCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);





#endif /* SRC_INCLUDE_APP_ENDPOINT_CFG_H_ */
