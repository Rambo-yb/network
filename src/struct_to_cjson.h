#ifndef __STRUCT_TO_CJSON_H__
#define __STRUCT_TO_CJSON_H__

#include "network_common.h"
#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 能力 **/
int StructToCjsonSupportFunction(void* st, cJSON** json);

/** 系统 **/
int StructToCjsonDeviceInfo(void* st, cJSON** json);

/** 配置 **/
int StructToCjsonSystemInfo(void* st, cJSON** json);

int StructToCjsonCameraChipInfo(void* st, cJSON** json);

int StructToCjsonPosition(void* st, cJSON** json);

int StructToCjsonCalibrationGun(void* st, cJSON** json);

int StructToCjsonPtzInfo(void* st, cJSON** json);

int StructToCjsonAreas(void* st, cJSON** json);

int StructToCjsonOtherConfig(void* st, cJSON** json);

int StructToCjsonNetworkInfo(void* st, cJSON** json);

int StructToCjsonAlgorithm(void* st, cJSON** json);


int StructToCjsonAlarmInfo(void* st, cJSON** json);

int StructToCjsonPeripheralInfo(void* st, cJSON** json);


#ifdef __cplusplus
};
#endif

#endif