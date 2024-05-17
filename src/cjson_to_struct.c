#include "common.h"
#include "cjson_common.h"
#include "check_common.h"

int CjsonToStructAreas(cJSON* json, Areas* areas) {
    CHECK_POINTER(json, -1);
    CHECK_POINTER(areas, -1);

    if (!cJSON_IsArray(json)) {
        LOG_ERR("json is not array!\n");
        return -1;
    }

    areas->area_num = cJSON_GetArraySize(json);
    for(int i = 0; i < areas->area_num && i < AREA_NUM_MAX; i++) {
        cJSON* area_item = cJSON_GetArrayItem(json, i);
        CHECK_POINTER_GO(area_item, end);

        CJSON_GET_STRING(area_item, "area_name", areas->area[i].area_name, sizeof(areas->area[i].area_name), end);
        CJSON_GET_NUMBER(area_item, "enable", areas->area[i].enable, sizeof(areas->area[i].enable), end);
        CJSON_GET_NUMBER(area_item, "point_num_max", areas->area[i].point_num_max, sizeof(areas->area[i].point_num_max), end);
        CJSON_GET_NUMBER(area_item, "point_num", areas->area[i].point_num, sizeof(areas->area[i].point_num), end);
        
        cJSON* points = cJSON_GetObjectItem(area_item, "point");
        CHECK_POINTER(points, -1);
        CHECK_BOOL(cJSON_IsArray(points), -1);

        for(int j = 0; j < areas->area[i].point_num_max && i < POINT_NUM_MAX; j++) {
            cJSON* point_item = cJSON_GetArrayItem(points, j);
            CHECK_POINTER_GO(point_item, end);

            CJSON_GET_NUMBER(point_item, "x", areas->area[i].point[j].x, sizeof(areas->area[i].point[j].x), end);
            CJSON_GET_NUMBER(point_item, "y", areas->area[i].point[j].y, sizeof(areas->area[i].point[j].y), end);
        }
    }
    
    return 0;
end:
    return -1;
}

int CjsonToStructPtzCtrl(cJSON* json, PtzCtrl* ptz_ctrl) {
    CHECK_POINTER(json, -1);
    CHECK_POINTER(ptz_ctrl, -1);

    CJSON_GET_NUMBER(json, "yaw", ptz_ctrl->yaw, sizeof(ptz_ctrl->yaw), end);
    CJSON_GET_NUMBER(json, "pitch", ptz_ctrl->pitch, sizeof(ptz_ctrl->pitch), end);
    CJSON_GET_NUMBER(json, "scan_mode", ptz_ctrl->scan_mode, sizeof(ptz_ctrl->scan_mode), end);
    CJSON_GET_NUMBER(json, "step", ptz_ctrl->step, sizeof(ptz_ctrl->step), end);
    CJSON_GET_NUMBER(json, "motor_enable", ptz_ctrl->motor_enable, sizeof(ptz_ctrl->motor_enable), end);
    
    return 0;
end:
    return -1;
}

int CjsonToStructChipCtrl(cJSON* json, ChipCtrl* chip_ctrl) {
    CHECK_POINTER(json, -1);
    CHECK_POINTER(chip_ctrl, -1);

    CJSON_GET_NUMBER(json, "contrast", chip_ctrl->contrast, sizeof(chip_ctrl->contrast), end);
    CJSON_GET_NUMBER(json, "brightness", chip_ctrl->brightness, sizeof(chip_ctrl->brightness), end);
    CJSON_GET_NUMBER(json, "polarity", chip_ctrl->polarity, sizeof(chip_ctrl->polarity), end);
    CJSON_GET_NUMBER(json, "bad_spot_remove", chip_ctrl->bad_spot_remove, sizeof(chip_ctrl->bad_spot_remove), end);
    CJSON_GET_NUMBER(json, "hot_spot_track", chip_ctrl->hot_spot_track, sizeof(chip_ctrl->hot_spot_track), end);
    
    return 0;
end:
    return -1;
}

int CjsonToStructOtherCtrl(cJSON* json, OtherCtrl* other_ctrl) {
    CHECK_POINTER(json, -1);
    CHECK_POINTER(other_ctrl, -1);

    CJSON_GET_NUMBER(json, "defog_by_heat_enable", other_ctrl->defog_by_heat_enable, sizeof(other_ctrl->defog_by_heat_enable), end);
    
    return 0;
end:
    return -1;
}

int CjsonToStructAlgorithemEnable(cJSON* json, AlgorithemEnable* algorithem_enable) {
    CHECK_POINTER(json, -1);
    CHECK_POINTER(algorithem_enable, -1);

    CJSON_GET_NUMBER(json, "detection_enable", algorithem_enable->detection_enable, sizeof(algorithem_enable->detection_enable), end);
    CJSON_GET_NUMBER(json, "tracking_enable", algorithem_enable->tracking_enable, sizeof(algorithem_enable->tracking_enable), end);
    CJSON_GET_NUMBER(json, "action_analyze_enable", algorithem_enable->action_analyze_enable, sizeof(algorithem_enable->action_analyze_enable), end);

    cJSON* tracking_object = cJSON_GetObjectItem(json, "tracking_object");
    CHECK_POINTER(tracking_object, -1);

    CJSON_GET_NUMBER(tracking_object, "id_num", algorithem_enable->tracking_object.id_num, sizeof(algorithem_enable->tracking_object.id_num), end);

    cJSON* ids = cJSON_GetObjectItem(tracking_object, "id");
    CHECK_POINTER(ids, -1);
    CHECK_BOOL(cJSON_IsArray(ids), -1);

    int id_arr_size = cJSON_GetArraySize(ids);
    for(int i = 0; i < id_arr_size && i < algorithem_enable->tracking_object.id_num && i < TRACK_ID_NUM_MAX; i++) {
        cJSON* obj_item = cJSON_GetArrayItem(ids, i);
        CHECK_POINTER_GO(obj_item, end);

        if (cJSON_IsNumber(obj_item)) {
            algorithem_enable->tracking_object.id[i] = cJSON_GetNumberValue(obj_item);
        }
    }
    
    return 0;
end:
    return -1;
}

int CjsonToStructAlarmInfo(cJSON* json, AlarmInfo* alarm_info) {
    CHECK_POINTER(json, -1);
    CHECK_POINTER(alarm_info, -1);

    CJSON_GET_NUMBER(json, "over_boundary", alarm_info->over_boundary, sizeof(alarm_info->over_boundary), end);
    CJSON_GET_NUMBER(json, "area_intrude", alarm_info->area_intrude, sizeof(alarm_info->area_intrude), end);
    CJSON_GET_NUMBER(json, "abnormal_action", alarm_info->abnormal_action, sizeof(alarm_info->abnormal_action), end);
    CJSON_GET_NUMBER(json, "fire_point_detection", alarm_info->fire_point_detection, sizeof(alarm_info->fire_point_detection), end);
    CJSON_GET_NUMBER(json, "temperature_abnormal", alarm_info->temperature_abnormal, sizeof(alarm_info->temperature_abnormal), end);
    CJSON_GET_NUMBER(json, "voltage_abnormal", alarm_info->voltage_abnormal, sizeof(alarm_info->voltage_abnormal), end);
    CJSON_GET_NUMBER(json, "ampere_abnormal", alarm_info->ampere_abnormal, sizeof(alarm_info->ampere_abnormal), end);
    
    return 0;
end:
    return -1;
}

int CjsonToStructPeripheralInfo(cJSON* json, PeripheralInfo* peripheral_info) {
    CHECK_POINTER(json, -1);
    CHECK_POINTER(peripheral_info, -1);

    CJSON_GET_NUMBER(json, "yaw", peripheral_info->yaw, sizeof(peripheral_info->yaw), end);
    CJSON_GET_NUMBER(json, "pitch", peripheral_info->pitch, sizeof(peripheral_info->pitch), end);
    CJSON_GET_NUMBER(json, "motor_temperature", peripheral_info->motor_temperature, sizeof(peripheral_info->motor_temperature), end);
    CJSON_GET_NUMBER(json, "input_voltage", peripheral_info->input_voltage, sizeof(peripheral_info->input_voltage), end);
    CJSON_GET_NUMBER(json, "working_ampere", peripheral_info->working_ampere, sizeof(peripheral_info->working_ampere), end);
    CJSON_GET_NUMBER(json, "longitude", peripheral_info->longitude, sizeof(peripheral_info->longitude), end);
    CJSON_GET_NUMBER(json, "latitude", peripheral_info->latitude, sizeof(peripheral_info->latitude), end);
    
    return 0;
end:
    return -1;
}
