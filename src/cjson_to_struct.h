#ifndef __CJSON_TO_STRUCT_H__
#define __CJSON_TO_STRUCT_H__

#include "common.h"
#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

int CjsonToStructAreas(cJSON* json, Areas* areas);

int CjsonToStructPtzCtrl(cJSON* json, PtzCtrl* ptz_ctrl);

int CjsonToStructChipCtrl(cJSON* json, ChipCtrl* chip_ctrl);

int CjsonToStructOtherCtrl(cJSON* json, OtherCtrl* other_ctrl);

int CjsonToStructAlgorithemEnable(cJSON* json, AlgorithemEnable* algorithem_enable);

int CjsonToStructAlarmInfo(cJSON* json, AlarmInfo* alarm_info);

int CjsonToStructPeripheralInfo(cJSON* json, PeripheralInfo* peripheral_info);

#ifdef __cplusplus
};
#endif

#endif