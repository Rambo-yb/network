#ifndef __STRUCT_TO_CJSON_H__
#define __STRUCT_TO_CJSON_H__

#include "network_common.h"
#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

int StructToCjsonAreas(void* st, cJSON** json_);

int StructToCjsonPtzCtrl(void* st, cJSON** json);

int StructToCjsonChipCtrl(void* st, cJSON** json);

int StructToCjsonOtherCtrl(void* st, cJSON** json);

int StructToCjsonAlgorithemEnable(void* st, cJSON** json);

int StructToCjsonAlarmInfo(void* st, cJSON** json);

int StructToCjsonPeripheralInfo(void* st, cJSON** json);


#ifdef __cplusplus
};
#endif

#endif