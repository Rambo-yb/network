#ifndef __CJSON_TO_STRUCT_H__
#define __CJSON_TO_STRUCT_H__

#include "network_common.h"
#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

int CjsonToStructAreas(cJSON* json, void* st);

int CjsonToStructPtzCtrl(cJSON* json, void* st);

int CjsonToStructChipCtrl(cJSON* json, void* st);

int CjsonToStructOtherCtrl(cJSON* json, void* st);

int CjsonToStructAlgorithmEnable(cJSON* json, void* st);

int CjsonToStructAlarmInfo(cJSON* json, void* st);

int CjsonToStructPeripheralInfo(cJSON* json, void* st);

#ifdef __cplusplus
};
#endif

#endif