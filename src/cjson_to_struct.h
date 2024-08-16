#ifndef __CJSON_TO_STRUCT_H__
#define __CJSON_TO_STRUCT_H__

#include "network_common.h"
#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 配置 **/
int CjsonToStructSystemInfo(cJSON* json, void* st);

int CjsonToStructCameraChipInfo(cJSON* json, void* st);

int CjsonToStructPosition(cJSON* json, void* st);

int CjsonToStructCalibrationGun(cJSON* json, void* st);

int CjsonToStructPtzInfo(cJSON* json, void* st);

int CjsonToStructAreas(cJSON* json, void* st);

int CjsonToStructOtherConfig(cJSON* json, void* st);

int CjsonToStructNetworkInfo(cJSON* json, void* st);

int CjsonToStructAlgorithm(cJSON* json, void* st);



int CjsonToStructAlarmInfo(cJSON* json, void* st);

int CjsonToStructPeripheralInfo(cJSON* json, void* st);

#ifdef __cplusplus
};
#endif

#endif