#include "struct_to_cjson.h"
#include "cjson_common.h"
#include "check_common.h"

int StructToCjsonAreas(void* st, cJSON** json) {
    CHECK_POINTER(st, -1);

    Areas* areas = (Areas*)st;
    cJSON* new_json = cJSON_CreateArray();
    CHECK_POINTER(new_json, -1);

    cJSON* area_item = NULL;
    cJSON* points = NULL;
    cJSON* point_item = NULL;
    for(int i = 0; i < areas->area_num && i < NET_AREA_NUM_MAX; i++) {
        area_item = cJSON_CreateObject();
        CHECK_POINTER_GO(area_item, end);
        
        CJSON_SET_STRING(area_item, "area_name", areas->area[i].area_name, end_area);
        CJSON_SET_NUMBER(area_item, "enable", areas->area[i].enable, end_area);
        CJSON_SET_NUMBER(area_item, "point_num_max", areas->area[i].point_num_max, end_area);
        CJSON_SET_NUMBER(area_item, "point_num", areas->area[i].point_num, end_area);

        points = cJSON_CreateArray();
        CHECK_POINTER_GO(points, end_area);

        for(int j = 0; j < areas->area[i].point_num && j < areas->area[i].point_num_max && j < NET_POINT_NUM_MAX; j++) {
            point_item = cJSON_CreateObject();
            CHECK_POINTER_GO(point_item, end_points);

            CJSON_SET_NUMBER(point_item, "x", areas->area[i].point[j].x, end_point);
            CJSON_SET_NUMBER(point_item, "y", areas->area[i].point[j].y, end_point);

            CHECK_BOOL_GO(cJSON_AddItemToArray(points, point_item), end_point);
        }

        CHECK_BOOL_GO(cJSON_AddItemToObject(area_item, "point", points), end_points);
        CHECK_BOOL_GO(cJSON_AddItemToArray(new_json, area_item), end_area);
    }

    *json = new_json;

    return 0;
end_point:
    cJSON_free(point_item);
end_points:
    cJSON_free(points);
end_area:
    cJSON_free(area_item);
end:
    cJSON_free(new_json);
    return -1;
}

int StructToCjsonPtzCtrl(void* st, cJSON** json) {
    CHECK_POINTER(st, -1);

    PtzCtrl* ptz_ctrl = (PtzCtrl*)st;
    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, -1);

    CJSON_SET_NUMBER(new_json, "motor_enable", ptz_ctrl->motor_enable, end);
    CJSON_SET_NUMBER(new_json, "scan_mode", ptz_ctrl->scan_mode, end);
    CJSON_SET_NUMBER(new_json, "pitch", ptz_ctrl->pitch, end);
    CJSON_SET_NUMBER(new_json, "step", ptz_ctrl->step, end);
    CJSON_SET_NUMBER(new_json, "speed", ptz_ctrl->speed, end);
    CJSON_SET_NUMBER(new_json, "zero_falg", ptz_ctrl->zero_falg, end);
    
    cJSON* sub_scan_json = cJSON_CreateObject();
    CHECK_POINTER_GO(sub_scan_json, end);
    CJSON_SET_NUMBER(sub_scan_json, "value_type", ptz_ctrl->constant_scan.value_type, end_sub_scan);
    CJSON_SET_NUMBER(sub_scan_json, "yaw", ptz_ctrl->constant_scan.yaw, end_sub_scan);
    
    cJSON* pix = cJSON_CreateObject();
    CHECK_POINTER_GO(pix, end_sub_scan);
    CJSON_SET_NUMBER(pix, "x", ptz_ctrl->constant_scan.pix.x, end_pix);
    CJSON_SET_NUMBER(pix, "y", ptz_ctrl->constant_scan.pix.y, end_pix);

    CHECK_BOOL_GO(cJSON_AddItemToObject(sub_scan_json, "pix", pix), end_pix);
    CHECK_BOOL_GO(cJSON_AddItemToObject(new_json, "constant_scan", sub_scan_json), end_sub_scan);

    sub_scan_json = cJSON_CreateObject();
    CHECK_POINTER_GO(sub_scan_json, end);
    CJSON_SET_NUMBER(sub_scan_json, "start_angle", ptz_ctrl->fan_scanning.start_angle, end_sub_scan);
    CJSON_SET_NUMBER(sub_scan_json, "end_angle", ptz_ctrl->fan_scanning.end_angle, end_sub_scan);
    CHECK_BOOL_GO(cJSON_AddItemToObject(new_json, "fan_scanning", sub_scan_json), end_sub_scan);

    *json = new_json;

    return 0;
end_pix:
    cJSON_free(pix);
end_sub_scan:
    cJSON_free(sub_scan_json);
end:
    cJSON_free(new_json);
    return -1;
}

int StructToCjsonChipCtrl(void* st, cJSON** json) {
    CHECK_POINTER(st, -1);

    ChipCtrl* chip_ctrl = (ChipCtrl*)st;
    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, -1);

    CJSON_SET_NUMBER(new_json, "contrast", chip_ctrl->contrast, end);
    CJSON_SET_NUMBER(new_json, "brightness", chip_ctrl->brightness, end);
    CJSON_SET_NUMBER(new_json, "polarity", chip_ctrl->polarity, end);
    CJSON_SET_NUMBER(new_json, "bad_spot_threshold", chip_ctrl->bad_spot_threshold, end);
    CJSON_SET_NUMBER(new_json, "bad_spot_oper", chip_ctrl->bad_spot_oper, end);
    CJSON_SET_NUMBER(new_json, "hot_spot_track", chip_ctrl->hot_spot_track, end);

    *json = new_json;
    return 0;
end:
    cJSON_free(new_json);
    return -1;
}

int StructToCjsonOtherCtrl(void* st, cJSON** json) {
    CHECK_POINTER(st, -1);

    OtherCtrl* other_ctrl = (OtherCtrl*)st;
    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, -1);

    CJSON_SET_NUMBER(new_json, "defog_by_heat_enable", other_ctrl->defog_by_heat_enable, end);

    *json = new_json;
    return 0;
end:
    cJSON_free(new_json);
    return -1;
}

int StructToCjsonAlgorithmEnable(void* st, cJSON** json) {
    CHECK_POINTER(st, -1);

    AlgorithmEnable* algorithm_enable = (AlgorithmEnable*)st;
    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, -1);

    cJSON* id = NULL;
    cJSON* tracking_object = NULL;
        
    CJSON_SET_NUMBER(new_json, "detection_enable", algorithm_enable->detection_enable, end);
    CJSON_SET_NUMBER(new_json, "tracking_enable", algorithm_enable->tracking_enable, end);
    CJSON_SET_NUMBER(new_json, "action_analyze_enable", algorithm_enable->action_analyze_enable, end);

    tracking_object = cJSON_CreateArray();
    CHECK_POINTER_GO(tracking_object, end);

    for(int i = 0; i < algorithm_enable->tracking_object.id_num; i++) {
        id = cJSON_CreateNumber(algorithm_enable->tracking_object.id[i]);
        CHECK_POINTER_GO(id, end_object);

        CHECK_BOOL_GO(cJSON_AddItemToArray(tracking_object, id), end_id);
    }
    CHECK_BOOL_GO(cJSON_AddItemToObject(new_json, "tracking_object", tracking_object), end_object);

    *json = new_json;
    return 0;

end_id:
    cJSON_free(id);
end_object:
    cJSON_free(tracking_object);
end:
    cJSON_free(new_json);
    return -1;
}

int StructToCjsonAlarmInfo(void* st, cJSON** json) {
    CHECK_POINTER(st, -1);

    AlarmInfo* alarm_info = (AlarmInfo*)st;
    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, -1);

    CJSON_SET_NUMBER(new_json, "over_boundary", alarm_info->over_boundary, end);
    CJSON_SET_NUMBER(new_json, "area_intrude", alarm_info->area_intrude, end);
    CJSON_SET_NUMBER(new_json, "abnormal_action", alarm_info->abnormal_action, end);
    CJSON_SET_NUMBER(new_json, "fire_point_detection", alarm_info->fire_point_detection, end);
    CJSON_SET_NUMBER(new_json, "temperature_abnormal", alarm_info->temperature_abnormal, end);
    CJSON_SET_NUMBER(new_json, "voltage_abnormal", alarm_info->voltage_abnormal, end);
    CJSON_SET_NUMBER(new_json, "ampere_abnormal", alarm_info->ampere_abnormal, end);

    *json = new_json;
    return 0;
end:
    cJSON_free(new_json);
    return -1;
}

int StructToCjsonPeripheralInfo(void* st, cJSON** json) {
    CHECK_POINTER(st, -1);

    PeripheralInfo* peripheral_info = (PeripheralInfo*)st;
    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, -1);

    CJSON_SET_NUMBER(new_json, "yaw", peripheral_info->yaw, end);
    CJSON_SET_NUMBER(new_json, "pitch", peripheral_info->pitch, end);
    CJSON_SET_NUMBER(new_json, "motor_temperature", peripheral_info->motor_temperature, end);
    CJSON_SET_NUMBER(new_json, "input_voltage", peripheral_info->input_voltage, end);
    CJSON_SET_NUMBER(new_json, "working_ampere", peripheral_info->working_ampere, end);
    CJSON_SET_NUMBER(new_json, "longitude", peripheral_info->longitude, end);
    CJSON_SET_NUMBER(new_json, "latitude", peripheral_info->latitude, end);

    *json = new_json;
    return 0;
end:
    cJSON_free(new_json);
    return -1;
}
