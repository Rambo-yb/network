#include "cjson_to_struct.h"
#include "cjson_common.h"
#include "check_common.h"

int CjsonToStructAreas(cJSON* json, void* st) {
    CHECK_POINTER(json, -1);
    CHECK_POINTER(st, -1);

    if (!cJSON_IsArray(json)) {
        LOG_ERR("json is not array!\n");
        return -1;
    }

    Areas* areas = (Areas*)st;
    areas->area_num = cJSON_GetArraySize(json);
    for(int i = 0; i < areas->area_num && i < NET_AREA_NUM_MAX; i++) {
        cJSON* area_item = cJSON_GetArrayItem(json, i);
        CHECK_POINTER_GO(area_item, end);

        CJSON_GET_STRING(area_item, "area_name", areas->area[i].area_name, sizeof(areas->area[i].area_name), end);
        CJSON_GET_NUMBER(area_item, "enable", areas->area[i].enable, sizeof(areas->area[i].enable), end);
        CJSON_GET_NUMBER(area_item, "point_num_max", areas->area[i].point_num_max, sizeof(areas->area[i].point_num_max), end);
        CJSON_GET_NUMBER(area_item, "point_num", areas->area[i].point_num, sizeof(areas->area[i].point_num), end);
        
        cJSON* points = cJSON_GetObjectItemCaseSensitive(area_item, "point");
        CHECK_POINTER(points, -1);
        CHECK_BOOL(cJSON_IsArray(points), -1);

        for(int j = 0; j < areas->area[i].point_num && j < areas->area[i].point_num_max && i < NET_POINT_NUM_MAX; j++) {
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

int CjsonToStructPtzCtrl(cJSON* json, void* st) {
    CHECK_POINTER(json, -1);
    CHECK_POINTER(st, -1);

    PtzCtrl* ptz_ctrl = (PtzCtrl*)st;
    CJSON_GET_NUMBER(json, "motor_enable", ptz_ctrl->motor_enable, sizeof(ptz_ctrl->motor_enable), end);
    CJSON_GET_NUMBER(json, "scan_mode", ptz_ctrl->scan_mode, sizeof(ptz_ctrl->scan_mode), end);
    CJSON_GET_NUMBER(json, "pitch", ptz_ctrl->pitch, sizeof(ptz_ctrl->pitch), end);
    CJSON_GET_NUMBER(json, "step", ptz_ctrl->step, sizeof(ptz_ctrl->step), end);
    CJSON_GET_NUMBER(json, "speed", ptz_ctrl->speed, sizeof(ptz_ctrl->speed), end);
    CJSON_GET_NUMBER(json, "zero_falg", ptz_ctrl->zero_falg, sizeof(ptz_ctrl->zero_falg), end);

    cJSON* constant_scan = cJSON_GetObjectItemCaseSensitive(json, "constant_scan");
    CHECK_POINTER(constant_scan, -1);
    CJSON_GET_NUMBER(constant_scan, "value_type", ptz_ctrl->constant_scan.value_type, sizeof(ptz_ctrl->constant_scan.value_type), end);
    CJSON_GET_NUMBER(constant_scan, "yaw", ptz_ctrl->constant_scan.yaw, sizeof(ptz_ctrl->constant_scan.yaw), end);

    cJSON* pix = cJSON_GetObjectItemCaseSensitive(constant_scan, "pix");
    CHECK_POINTER(pix, -1);
    CJSON_GET_NUMBER(pix, "x", ptz_ctrl->constant_scan.pix.x, sizeof(ptz_ctrl->constant_scan.pix.x), end);
    CJSON_GET_NUMBER(pix, "y", ptz_ctrl->constant_scan.pix.y, sizeof(ptz_ctrl->constant_scan.pix.y), end);

    cJSON* fan_scanning = cJSON_GetObjectItemCaseSensitive(json, "fan_scanning");
    CHECK_POINTER(fan_scanning, -1);
    CJSON_GET_NUMBER(fan_scanning, "start_angle", ptz_ctrl->fan_scanning.start_angle, sizeof(ptz_ctrl->fan_scanning.start_angle), end);
    CJSON_GET_NUMBER(fan_scanning, "end_angle", ptz_ctrl->fan_scanning.end_angle, sizeof(ptz_ctrl->fan_scanning.end_angle), end);
    
    return 0;
end:
    return -1;
}

int CjsonToStructChipCtrl(cJSON* json, void* st) {
    CHECK_POINTER(json, -1);
    CHECK_POINTER(st, -1);
    
    ChipCtrl* chip_ctrl = (ChipCtrl*)st;
    CJSON_GET_NUMBER(json, "contrast", chip_ctrl->contrast, sizeof(chip_ctrl->contrast), end);
    CJSON_GET_NUMBER(json, "brightness", chip_ctrl->brightness, sizeof(chip_ctrl->brightness), end);
    CJSON_GET_NUMBER(json, "polarity", chip_ctrl->polarity, sizeof(chip_ctrl->polarity), end);
    CJSON_GET_NUMBER(json, "bad_spot_threshold", chip_ctrl->bad_spot_threshold, sizeof(chip_ctrl->bad_spot_threshold), end);
    CJSON_GET_NUMBER(json, "bad_spot_oper", chip_ctrl->bad_spot_oper, sizeof(chip_ctrl->bad_spot_oper), end);
    CJSON_GET_NUMBER(json, "hot_spot_track", chip_ctrl->hot_spot_track, sizeof(chip_ctrl->hot_spot_track), end);
    
    return 0;
end:
    return -1;
}

int CjsonToStructOtherCtrl(cJSON* json, void* st) {
    CHECK_POINTER(json, -1);
    CHECK_POINTER(st, -1);

    OtherCtrl* other_ctrl = (OtherCtrl*)st;
    CJSON_GET_NUMBER(json, "defog_by_heat_enable", other_ctrl->defog_by_heat_enable, sizeof(other_ctrl->defog_by_heat_enable), end);
    
    return 0;
end:
    return -1;
}

int CjsonToStructAlgorithmEnable(cJSON* json, void* st) {
    CHECK_POINTER(json, -1);
    CHECK_POINTER(st, -1);

    AlgorithmEnable* algorithm_enable = (AlgorithmEnable*)st;
    CJSON_GET_NUMBER(json, "detection_enable", algorithm_enable->detection_enable, sizeof(algorithm_enable->detection_enable), end);
    CJSON_GET_NUMBER(json, "tracking_enable", algorithm_enable->tracking_enable, sizeof(algorithm_enable->tracking_enable), end);
    CJSON_GET_NUMBER(json, "action_analyze_enable", algorithm_enable->action_analyze_enable, sizeof(algorithm_enable->action_analyze_enable), end);

    cJSON* tracking_object = cJSON_GetObjectItemCaseSensitive(json, "tracking_object");
    CHECK_POINTER(tracking_object, -1);
    CHECK_BOOL(cJSON_IsArray(tracking_object), -1);

    algorithm_enable->tracking_object.id_num = cJSON_GetArraySize(tracking_object);
    for(int i = 0; i < algorithm_enable->tracking_object.id_num && i < NET_TRACK_ID_NUM_MAX; i++) {
        cJSON* obj_item = cJSON_GetArrayItem(tracking_object, i);
        CHECK_POINTER_GO(obj_item, end);

        if (cJSON_IsNumber(obj_item)) {
            algorithm_enable->tracking_object.id[i] = cJSON_GetNumberValue(obj_item);
        }
    }
    
    return 0;
end:
    return -1;
}

int CjsonToStructAlarmInfo(cJSON* json, void* st) {
    CHECK_POINTER(json, -1);
    CHECK_POINTER(st, -1);

    AlarmInfo* alarm_info = (AlarmInfo*)st;
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

int CjsonToStructPeripheralInfo(cJSON* json, void* st) {
    CHECK_POINTER(json, -1);
    CHECK_POINTER(st, -1);

    PeripheralInfo* peripheral_info = (PeripheralInfo*)st;
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
