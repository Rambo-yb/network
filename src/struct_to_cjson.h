#ifndef __STRUCT_TO_CJSON_H__
#define __STRUCT_TO_CJSON_H__

#include "common.h"
#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

int StructToCjsonAreas(Areas* areas, cJSON* json);

int StructToCjsonPtzCtrl(PtzCtrl* ptz_ctrl, cJSON* json);

int StructToCjsonChipCtrl(ChipCtrl* chip_ctrl, cJSON* json);

int StructToCjsonOtherCtrl(OtherCtrl* other_ctrl, cJSON* json);

int StructToCjsonAlgorithemEnable(AlgorithemEnable* algorithem_enable, cJSON* json);

int StructToCjsonAlarmInfo(AlarmInfo* alarm_info, cJSON* json);

int StructToCjsonPeripheralInfo(PeripheralInfo* peripheral_info, cJSON* json);


#ifdef __cplusplus
};
#endif

#endif