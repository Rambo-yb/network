#ifndef __STRUCT_TO_CJSON_H__
#define __STRUCT_TO_CJSON_H__

#include "network_common.h"
#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 能力 **/
int StructToCjsonNetworkSupportFunction(void* st, cJSON** json);

/** 系统 **/
int StructToCjsonNetworkDeviceInfo(void* st, cJSON** json);

/** 配置 **/
int StructToCjsonNetworkSystemInfo(void* st, cJSON** json);

int StructToCjsonNetworkCameraChipInfo(void* st, cJSON** json);

int StructToCjsonNetworkPosition(void* st, cJSON** json);

int StructToCjsonNetworkCalibrationGun(void* st, cJSON** json);

int StructToCjsonNetworkPtzInfo(void* st, cJSON** json);

int StructToCjsonNetworkAreas(void* st, cJSON** json);

int StructToCjsonNetworkOtherConfig(void* st, cJSON** json);

int StructToCjsonNetworkNetworkInfo(void* st, cJSON** json);

int StructToCjsonNetworkAlgorithm(void* st, cJSON** json);


int StructToCjsonNetworkAlarmInfo(void* st, cJSON** json);

int StructToCjsonNetworkPeripheralInfo(void* st, cJSON** json);


#ifdef __cplusplus
};
#endif

#endif