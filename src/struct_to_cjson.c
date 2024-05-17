#include "common.h"
#include "cjson_common.h"
#include "check_common.h"

int StructToCjsonAreas(Areas* areas, cJSON** json) {
    CHECK_POINTER(areas, -1);

    cJSON* new_json = cJSON_CreateArray();
    CHECK_POINTER(new_json, -1);

    cJSON* area_item = NULL;
    cJSON* points = NULL;
    cJSON* point_item = NULL;
    for(int i = 0; i < areas->area_num && i < AREA_NUM_MAX; i++) {
        area_item = cJSON_CreateObject();
        CHECK_POINTER_GO(area_item, end);
        
        CJSON_SET_STRING(area_item, "area_name", areas->area[i].area_name, end_area);
        CJSON_SET_NUMBER(area_item, "enable", areas->area[i].enable, end_area);
        CJSON_SET_NUMBER(area_item, "point_num_max", areas->area[i].point_num_max, end_area);
        CJSON_SET_NUMBER(area_item, "point_num", areas->area[i].point_num, end_area);

        points = cJSON_CreateArray();
        CHECK_POINTER_GO(points, end_area);

        for(int j = 0; j < areas->area[i].point_num_max; j++) {
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

int StructToCjsonPtzCtrl(PtzCtrl* ptz_ctrl, cJSON** json) {
    CHECK_POINTER(ptz_ctrl, -1);

    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, -1);

    CJSON_SET_NUMBER(new_json, "yaw", ptz_ctrl->yaw, end);
    CJSON_SET_NUMBER(new_json, "pitch", ptz_ctrl->pitch, end);
    CJSON_SET_NUMBER(new_json, "scan_mode", ptz_ctrl->scan_mode, end);
    CJSON_SET_NUMBER(new_json, "step", ptz_ctrl->step, end);
    CJSON_SET_NUMBER(new_json, "motor_enable", ptz_ctrl->motor_enable, end);

    *json = new_json;

    return 0;
end:
    cJSON_free(new_json);
    return -1;
}

int StructToCjsonChipCtrl(ChipCtrl* chip_ctrl, cJSON** json) {
    CHECK_POINTER(chip_ctrl, -1);

    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, -1);

    CJSON_SET_NUMBER(new_json, "contrast", chip_ctrl->contrast, end);
    CJSON_SET_NUMBER(new_json, "brightness", chip_ctrl->brightness, end);
    CJSON_SET_NUMBER(new_json, "polarity", chip_ctrl->polarity, end);
    CJSON_SET_NUMBER(new_json, "bad_spot_remove", chip_ctrl->bad_spot_remove, end);
    CJSON_SET_NUMBER(new_json, "hot_spot_track", chip_ctrl->hot_spot_track, end);

    *json = new_json;
    return 0;
end:
    cJSON_free(new_json);
    return -1;
}

int StructToCjsonOtherCtrl(OtherCtrl* other_ctrl, cJSON** json) {
    CHECK_POINTER(other_ctrl, -1);

    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, -1);

    CJSON_SET_NUMBER(new_json, "defog_by_heat_enable", other_ctrl->defog_by_heat_enable, end);

    *json = new_json;
    return 0;
end:
    cJSON_free(new_json);
    return -1;
}

int StructToCjsonAlgorithemEnable(AlgorithemEnable* algorithem_enable, cJSON** json) {
    CHECK_POINTER(algorithem_enable, -1);

    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, -1);

    cJSON* ids = NULL;
    cJSON* id = NULL;
    cJSON* tracking_object = NULL;
        
    CJSON_SET_NUMBER(new_json, "detection_enable", algorithem_enable->detection_enable, end);
    CJSON_SET_NUMBER(new_json, "tracking_enable", algorithem_enable->tracking_enable, end);
    CJSON_SET_NUMBER(new_json, "action_analyze_enable", algorithem_enable->action_analyze_enable, end);

    tracking_object = cJSON_CreateObject();
    CHECK_POINTER_GO(tracking_object, end);
    CJSON_SET_NUMBER(tracking_object, "id_num", algorithem_enable->tracking_object.id_num, end_object);

    ids = cJSON_CreateArray();
    CHECK_POINTER_GO(ids, end_object);
    for(int i = 0; i < algorithem_enable->tracking_object.id_num; i++) {
        id = cJSON_CreateNumber(algorithem_enable->tracking_object.id[i]);
        CHECK_POINTER_GO(id, end_ids);

        CHECK_BOOL_GO(cJSON_AddItemToArray(ids, id), end_id);
    }
    CHECK_BOOL_GO(cJSON_AddItemToObject(tracking_object, "id", ids), end_ids);
    CHECK_BOOL_GO(cJSON_AddItemToObject(new_json, "tracking_object", tracking_object), end_object);

    *json = new_json;
    return 0;

end_id:
    cJSON_free(id);
end_ids:
    cJSON_free(ids);
end_object:
    cJSON_free(tracking_object);
end:
    cJSON_free(new_json);
    return -1;
}

int StructToCjsonAlarmInfo(AlarmInfo* alarm_info, cJSON** json) {
    CHECK_POINTER(alarm_info, -1);

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

int StructToCjsonPeripheralInfo(PeripheralInfo* peripheral_info, cJSON** json) {
    CHECK_POINTER(peripheral_info, -1);

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
