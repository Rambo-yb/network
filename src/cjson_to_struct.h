#ifndef __CJSON_TO_STRUCT_H__
#define __CJSON_TO_STRUCT_H__

#include "network_common.h"
#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 配置 **/
int CjsonToStructNetworkSystemInfo(cJSON* json, void* st);

int CjsonToStructNetworkCameraChipInfo(cJSON* json, void* st);

int CjsonToStructNetworkPosition(cJSON* json, void* st);

int CjsonToStructNetworkCalibrationGun(cJSON* json, void* st);

int CjsonToStructNetworkPtzInfo(cJSON* json, void* st);

int CjsonToStructNetworkAreas(cJSON* json, void* st);

int CjsonToStructNetworkOtherConfig(cJSON* json, void* st);

int CjsonToStructNetworkNetworkInfo(cJSON* json, void* st);

int CjsonToStructNetworkAlgorithm(cJSON* json, void* st);



int CjsonToStructNetworkAlarmInfo(cJSON* json, void* st);

int CjsonToStructNetworkPeripheralInfo(cJSON* json, void* st);

#ifdef __cplusplus
};
#endif

#endif