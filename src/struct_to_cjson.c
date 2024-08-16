#include "struct_to_cjson.h"
#include "cjson_common.h"
#include "check_common.h"

#define CJSON_SET_MASK(json, key, value, end) \
    do {    \
        char mask[16] = {0};    \
        snprintf(mask, sizeof(mask), "0x%08lld", value);    \
        CJSON_SET_STRING(json, key, mask, end); \
    }while (0);

#define CJSON_SET_SUB_OBJECT(json, key, st, func, end)    \
    do {    \
        cJSON* sub_json = NULL; \
        CHECK_LT(func(&st, &sub_json), 0, end);   \
        CHECK_BOOL(cJSON_AddItemToObject(json, key, sub_json), cJSON_free(sub_json); end);  \
    }while(0);

/** 能力 **/
static int StructToCjsonFunctionSoap(void* st, cJSON** json) {
    CHECK_POINTER(st, return -1);
    CHECK_POINTER(json, return -1);

    FunctionSoap* func_soap = (FunctionSoap*)st;
    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, return -1);

    CJSON_SET_NUMBER(new_json, "support_state", func_soap->support_state, goto end);
    CJSON_SET_NUMBER(new_json, "max", func_soap->max, goto end);
    CJSON_SET_NUMBER(new_json, "min", func_soap->min, goto end);

    *json = new_json;
    return 0;
end:
    cJSON_free(new_json);
    return -1;
}

static int StructToCjsonSystemFunction(void* st, cJSON** json) {
    CHECK_POINTER(st, return -1);
    CHECK_POINTER(json, return -1);

    SystemFunction* system_func = (SystemFunction*)st;
    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, return -1);

    CJSON_SET_MASK(new_json, "language_support_mask", system_func->language_support_mask, goto end);
    CJSON_SET_NUMBER(new_json, "position_function_support", system_func->position_function_support, goto end);
    CJSON_SET_NUMBER(new_json, "ptz_function_support", system_func->ptz_function_support, goto end);
    CJSON_SET_NUMBER(new_json, "area_function_support", system_func->area_function_support, goto end);
    CJSON_SET_NUMBER(new_json, "onvif_protocol_support", system_func->onvif_protocol_support, goto end);
    CJSON_SET_NUMBER(new_json, "ipv6_protocol_support", system_func->ipv6_protocol_support, goto end);

    *json = new_json;
    return 0;
end:
    cJSON_free(new_json);
    return -1;
}

static int StructToCjsonCameraChipFunction(void* st, cJSON** json) {
    CHECK_POINTER(st, return -1);
    CHECK_POINTER(json, return -1);

    CameraChipFunction* camera_chip_func = (CameraChipFunction*)st;
    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, return -1);

    CJSON_SET_NUMBER(new_json, "brightness_support", camera_chip_func->brightness_support, goto end);
    CJSON_SET_NUMBER(new_json, "contrast_support", camera_chip_func->contrast_support, goto end);
    CJSON_SET_MASK(new_json, "pseudo_color_support_mask", camera_chip_func->pseudo_color_support_mask, goto end);
    CJSON_SET_NUMBER(new_json, "sharpening_support", camera_chip_func->sharpening_support, goto end);
    CJSON_SET_NUMBER(new_json, "central_enhance_support", camera_chip_func->central_enhance_support, goto end);
    CJSON_SET_NUMBER(new_json, "detail_enhance_support", camera_chip_func->detail_enhance_support, goto end);
    CJSON_SET_NUMBER(new_json, "electronic_zoom_support", camera_chip_func->electronic_zoom_support, goto end);
    CJSON_SET_NUMBER(new_json, "target_recognition_support", camera_chip_func->target_recognition_support, goto end);
    CJSON_SET_NUMBER(new_json, "background_calibration_support", camera_chip_func->background_calibration_support, goto end);
    CJSON_SET_NUMBER(new_json, "hot_spot_tracking_support", camera_chip_func->hot_spot_tracking_support, goto end);
    CJSON_SET_MASK(new_json, "picture_in_picture_support_mask", camera_chip_func->picture_in_picture_support_mask, goto end);
    CJSON_SET_NUMBER(new_json, "outdoor_mode_support", camera_chip_func->outdoor_mode_support, goto end);
    CJSON_SET_MASK(new_json, "fusion_mode_support_mask", camera_chip_func->fusion_mode_support_mask, goto end);
    CJSON_SET_NUMBER(new_json, "shutter_calibration_support", camera_chip_func->shutter_calibration_support, goto end);
    CJSON_SET_SUB_OBJECT(new_json, "bad_pix_threshold_support", camera_chip_func->bad_pix_threshold_support, StructToCjsonFunctionSoap, goto end);

    *json = new_json;
    return 0;
end:
    cJSON_free(new_json);
    return -1;
}

static int StructToCjsonCalibrationGunFunction(void* st, cJSON** json) {
    CHECK_POINTER(st, return -1);
    CHECK_POINTER(json, return -1);

    CalibrationGunFunction* calibration_gun_func = (CalibrationGunFunction*)st;
    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, return -1);

    CJSON_SET_NUMBER(new_json, "support_state", calibration_gun_func->support_state, goto end);
    CJSON_SET_NUMBER(new_json, "gun_type_support", calibration_gun_func->gun_type_support, goto end);
    CJSON_SET_MASK(new_json, "graticule_style_support_mask", calibration_gun_func->graticule_style_support_mask, goto end);
    CJSON_SET_MASK(new_json, "color_support_mask", calibration_gun_func->color_support_mask, goto end);

    *json = new_json;
    return 0;
end:
    cJSON_free(new_json);
    return -1;
}

static int StructToCjsonOtherFunction(void* st, cJSON** json) {
    CHECK_POINTER(st, return -1);
    CHECK_POINTER(json, return -1);

    OtherFunction* other_func = (OtherFunction*)st;
    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, return -1);

    CJSON_SET_NUMBER(new_json, "defog_by_heat_support", other_func->defog_by_heat_support, goto end);
    CJSON_SET_NUMBER(new_json, "heat_support", other_func->heat_support, goto end);
    CJSON_SET_NUMBER(new_json, "fan_support", other_func->fan_support, goto end);
    CJSON_SET_NUMBER(new_json, "electronic_compass_support", other_func->electronic_compass_support, goto end);
    CJSON_SET_NUMBER(new_json, "red_dot_support", other_func->red_dot_support, goto end);
    CJSON_SET_NUMBER(new_json, "hdmi_support", other_func->hdmi_support, goto end);

    *json = new_json;
    return 0;
end:
    cJSON_free(new_json);
    return -1;
}

static int StructToCjsonAlgorithmFunction(void* st, cJSON** json) {
    CHECK_POINTER(st, return -1);
    CHECK_POINTER(json, return -1);

    AlgorithmFunction* algorithm_func = (AlgorithmFunction*)st;
    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, return -1);

    CJSON_SET_NUMBER(new_json, "object_detection_support", algorithm_func->object_detection_support, goto end);
    CJSON_SET_NUMBER(new_json, "tracking_support", algorithm_func->tracking_support, goto end);
    CJSON_SET_NUMBER(new_json, "action_analyze_support", algorithm_func->action_analyze_support, goto end);
    CJSON_SET_NUMBER(new_json, "splicing_support", algorithm_func->splicing_support, goto end);
    CJSON_SET_NUMBER(new_json, "fire_point_support", algorithm_func->fire_point_support, goto end);

    *json = new_json;
    return 0;
end:
    cJSON_free(new_json);
    return -1;
}

int StructToCjsonSupportFunction(void* st, cJSON** json) {
    CHECK_POINTER(st, return -1);
    CHECK_POINTER(json, return -1);

    SupportFunction* support_func = (SupportFunction*)st;
    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, return -1);

    CJSON_SET_SUB_OBJECT(new_json, "system_function", support_func->system_function, StructToCjsonSystemFunction, goto end);
    CJSON_SET_SUB_OBJECT(new_json, "camera_chip_function", support_func->camera_chip_function, StructToCjsonCameraChipFunction, goto end);
    CJSON_SET_SUB_OBJECT(new_json, "calibration_gun_function", support_func->calibration_gun_function, StructToCjsonCalibrationGunFunction, goto end);
    CJSON_SET_SUB_OBJECT(new_json, "other_function", support_func->other_function, StructToCjsonOtherFunction, goto end);
    CJSON_SET_SUB_OBJECT(new_json, "algorithm_function", support_func->algorithm_function, StructToCjsonAlgorithmFunction, goto end);

    *json = new_json;
    return 0;
end:
    cJSON_free(new_json);
    return -1;
}

/** 系统 **/
int StructToCjsonDeviceInfo(void* st, cJSON** json) {
    CHECK_POINTER(st, return -1);
    CHECK_POINTER(json, return -1);

    DeviceInfo* device_info = (DeviceInfo*)st;
    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, return -1);

    CJSON_SET_STRING(new_json, "soft_version", device_info->soft_version, goto end);
    CJSON_SET_STRING(new_json, "hard_version", device_info->soft_version, goto end);
    CJSON_SET_STRING(new_json, "serial_number", device_info->soft_version, goto end);

    *json = new_json;
    return 0;
end:
    cJSON_free(new_json);
    return -1;
}

/** 配置 **/
int StructToCjsonSystemInfo(void* st, cJSON** json) {
    CHECK_POINTER(st, return -1);
    CHECK_POINTER(json, return -1);

    SystemInfo* system_info = (SystemInfo*)st;
    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, return -1);

    CJSON_SET_STRING(new_json, "language", system_info->language, goto end);
    CJSON_SET_STRING(new_json, "time_zone", system_info->time_zone, goto end);
    CJSON_SET_NUMBER(new_json, "time_format", system_info->time_format, goto end);
    CJSON_SET_NUMBER(new_json, "summer_time_enable", system_info->summer_time_enable, goto end);

    *json = new_json;
    return 0;
end:
    cJSON_free(new_json);
    return -1;
}

int StructToCjsonCameraChipInfo(void* st, cJSON** json) {
    CHECK_POINTER(st, return -1);
    CHECK_POINTER(json, return -1);

    CameraChipInfo* camera_chip_info = (CameraChipInfo*)st;
    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, return -1);

    CJSON_SET_NUMBER(new_json, "brigthness", camera_chip_info->brigthness, goto end);
    CJSON_SET_NUMBER(new_json, "contrast", camera_chip_info->contrast, goto end);
    CJSON_SET_NUMBER(new_json, "pseudo_color", camera_chip_info->pseudo_color, goto end);
    CJSON_SET_NUMBER(new_json, "sharpening", camera_chip_info->sharpening, goto end);
    CJSON_SET_NUMBER(new_json, "central_enhance", camera_chip_info->central_enhance, goto end);
    CJSON_SET_NUMBER(new_json, "detail_enhance", camera_chip_info->detail_enhance, goto end);
    CJSON_SET_NUMBER(new_json, "electronic_zoom", camera_chip_info->electronic_zoom, goto end);
    CJSON_SET_NUMBER(new_json, "target_recognition", camera_chip_info->target_recognition, goto end);
    CJSON_SET_NUMBER(new_json, "background_calibration", camera_chip_info->background_calibration, goto end);
    CJSON_SET_NUMBER(new_json, "hot_spot_tracking", camera_chip_info->hot_spot_tracking, goto end);
    CJSON_SET_NUMBER(new_json, "picture_in_picture", camera_chip_info->picture_in_picture, goto end);
    CJSON_SET_NUMBER(new_json, "outdoor_mode", camera_chip_info->outdoor_mode, goto end);
    CJSON_SET_NUMBER(new_json, "fusion_mod", camera_chip_info->fusion_mod, goto end);
    CJSON_SET_NUMBER(new_json, "shutter_calibration", camera_chip_info->shutter_calibration, goto end);
    CJSON_SET_NUMBER(new_json, "bad_pix_threshold", camera_chip_info->bad_pix_threshold, goto end);

    *json = new_json;
    return 0;
end:
    cJSON_free(new_json);
    return -1;
}

int StructToCjsonPosition(void* st, cJSON** json) {
    CHECK_POINTER(st, return -1);
    CHECK_POINTER(json, return -1);

    Position* position = (Position*)st;
    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, return -1);

    CJSON_SET_NUMBER(new_json, "position_mode", position->position_mode, goto end);
    CJSON_SET_NUMBER(new_json, "external_position", position->external_position, goto end);
    CJSON_SET_NUMBER(new_json, "target_guidance", position->target_guidance, goto end);
    CJSON_SET_NUMBER(new_json, "first_and_last_target", position->first_and_last_target, goto end);
    CJSON_SET_NUMBER(new_json, "multi_point_position", position->multi_point_position, goto end);

    *json = new_json;
    return 0;
end:
    cJSON_free(new_json);
    return -1;
}

static int StructToCjsonPoint(void* st, cJSON** json) {
    CHECK_POINTER(st, return -1);
    CHECK_POINTER(json, return -1);

    Point* point = (Point*)st;
    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, return -1);

    CJSON_SET_NUMBER(new_json, "x", point->x, goto end);
    CJSON_SET_NUMBER(new_json, "y", point->y, goto end);
    
    *json = new_json;
    return 0;
end:
    cJSON_free(new_json);
    return -1;
}

int StructToCjsonCalibrationGun(void* st, cJSON** json) {
    CHECK_POINTER(st, return -1);
    CHECK_POINTER(json, return -1);

    CalibrationGun* calibration_gun = (CalibrationGun*)st;
    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, return -1);

    CJSON_SET_STRING(new_json, "gun_type", calibration_gun->gun_type, goto end);
    CJSON_SET_NUMBER(new_json, "colgraticule_style", calibration_gun->colgraticule_style, goto end);
    CJSON_SET_NUMBER(new_json, "color", calibration_gun->color, goto end);
    CJSON_SET_SUB_OBJECT(new_json, "coordinate", calibration_gun->coordinate, StructToCjsonPoint, goto end);

    *json = new_json;
    return 0;
end:
    cJSON_free(new_json);
    return -1;
}

static int StructToCjsonConstantScan(void* st, cJSON** json) {
    CHECK_POINTER(st, return -1);
    CHECK_POINTER(json, return -1);

    ConstantScan* constant_scan = (ConstantScan*)st;
    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, return -1);

    CJSON_SET_NUMBER(new_json, "deflection_mode", constant_scan->deflection_mode, goto end);
    CJSON_SET_NUMBER(new_json, "yaw", constant_scan->yaw, goto end);
    CJSON_SET_SUB_OBJECT(new_json, "pix", constant_scan->pix, StructToCjsonPoint, goto end);

    *json = new_json;
    return 0;
end:
    cJSON_free(new_json);
    return -1;
}

static int StructToCjsonFanScanning(void* st, cJSON** json) {
    CHECK_POINTER(st, return -1);
    CHECK_POINTER(json, return -1);

    FanScanning* fan_scanning = (FanScanning*)st;
    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, return -1);

    CJSON_SET_NUMBER(new_json, "start_angle", fan_scanning->start_angle, goto end);
    CJSON_SET_NUMBER(new_json, "end_angle", fan_scanning->end_angle, goto end);

    *json = new_json;
    return 0;
end:
    cJSON_free(new_json);
    return -1;
}

int StructToCjsonPtzInfo(void* st, cJSON** json) {
    CHECK_POINTER(st, return -1);
    CHECK_POINTER(json, return -1);

    PtzInfo* ptz_info = (PtzInfo*)st;
    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, return -1);
    CJSON_SET_NUMBER(new_json, "motor_enable", ptz_info->motor_enable, goto end);
    CJSON_SET_NUMBER(new_json, "scan_mode", ptz_info->scan_mode, goto end);
    CJSON_SET_SUB_OBJECT(new_json, "constant_scan", ptz_info->constant_scan, StructToCjsonConstantScan, goto end);
    CJSON_SET_SUB_OBJECT(new_json, "fan_scanning", ptz_info->fan_scanning, StructToCjsonFanScanning, goto end);
    CJSON_SET_NUMBER(new_json, "pitch", ptz_info->pitch, goto end);
    CJSON_SET_NUMBER(new_json, "step", ptz_info->step, goto end);
    CJSON_SET_NUMBER(new_json, "speed", ptz_info->speed, goto end);

    *json = new_json;
    return 0;
end:
    cJSON_free(new_json);
    return -1;
}

static int StructToCjsonArea(void* st, cJSON** json) {
    CHECK_POINTER(st, return -1);
    CHECK_POINTER(json, return -1);

    Area* area = (Area*)st;
    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, return -1);

    CJSON_SET_STRING(new_json, "area_name", area->area_name, goto end);
    CJSON_SET_NUMBER(new_json, "enable", area->enable, goto end);
    CJSON_SET_NUMBER(new_json, "point_num_max", area->point_num_max, goto end);
    CJSON_SET_NUMBER(new_json, "point_num", area->point_num, goto end);

    cJSON* point = cJSON_CreateArray();
    CHECK_POINTER(point, goto end);

    for(int i = 0; i < area->point_num && i < area->point_num_max && i < NET_POINT_NUM_MAX; i++) {
        cJSON* point_item = NULL;
        CHECK_LT(StructToCjsonPoint(&area->point[i], &point_item), 0, cJSON_free(point);goto end);
        CHECK_BOOL(cJSON_AddItemToArray(point, point_item), cJSON_free(point_item);cJSON_free(point);goto end);
    }

    CHECK_BOOL(cJSON_AddItemToObject(new_json, "point", point), cJSON_free(point);goto end);
    
    *json = new_json;
    return 0;
end:
    cJSON_free(new_json);
    return -1; 
}

int StructToCjsonAreas(void* st, cJSON** json) {
    CHECK_POINTER(st, return -1);
    CHECK_POINTER(json, return -1);

    Areas* areas = (Areas*)st;
    cJSON* new_json = cJSON_CreateArray();
    CHECK_POINTER(new_json, -1);

    for(int i = 0; i < areas->area_num && i < NET_AREA_NUM_MAX; i++) {
        cJSON* area_item = NULL;
        CHECK_LT(StructToCjsonArea(&areas[i], &area_item), 0, goto end);
        CHECK_BOOL(cJSON_AddItemToArray(new_json, area_item), cJSON_free(area_item);goto end);
    }

    *json = new_json;
    return 0;
end:
    cJSON_free(new_json);
    return -1;
}

static int StructToCjsonRedDotInfo(void* st, cJSON** json) {
    CHECK_POINTER(st, return -1);
    CHECK_POINTER(json, return -1);

    RedDotInfo* red_dot_info = (RedDotInfo*)st;
    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, -1);

    CJSON_SET_NUMBER(new_json, "enable", red_dot_info->enable, goto end);
    CJSON_SET_SUB_OBJECT(new_json, "coordinate", red_dot_info->coordinate, StructToCjsonPoint, goto end);

    *json = new_json;
    return 0;
end:
    cJSON_free(new_json);
    return -1;
}

int StructToCjsonOtherConfig(void* st, cJSON** json) {
    CHECK_POINTER(st, return -1);
    CHECK_POINTER(json, return -1);

    OtherConfig* other_config = (OtherConfig*)st;
    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, -1);

    CJSON_SET_NUMBER(new_json, "defog_by_heat_enable", other_config->defog_by_heat_enable, goto end);
    CJSON_SET_NUMBER(new_json, "heat_enable", other_config->heat_enable, goto end);
    CJSON_SET_NUMBER(new_json, "fan_enable", other_config->fan_enable, goto end);
    CJSON_SET_NUMBER(new_json, "electronic_compass_enable", other_config->electronic_compass_enable, goto end);
    CJSON_SET_SUB_OBJECT(new_json, "red_dot", other_config->red_dot, StructToCjsonRedDotInfo, goto end);
    CJSON_SET_NUMBER(new_json, "hdmi_enable", other_config->hdmi_enable, goto end);

    *json = new_json;
    return 0;
end:
    cJSON_free(new_json);
    return -1;
}

int StructToCjsonNetworkInfo(void* st, cJSON** json) {
    CHECK_POINTER(st, return -1);
    CHECK_POINTER(json, return -1);

    NetworkInfo* network_info = (NetworkInfo*)st;
    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, -1);
        
    CJSON_SET_NUMBER(new_json, "dhcp_enable", network_info->dhcp_enable, goto end);
    CJSON_SET_NUMBER(new_json, "ipv4_enable", network_info->ipv4_enable, goto end);
    CJSON_SET_STRING(new_json, "ipv4_addr", network_info->ipv4_addr, goto end);
    CJSON_SET_STRING(new_json, "ipv4_mask", network_info->ipv4_mask, goto end);
    CJSON_SET_STRING(new_json, "ipv4_gateway", network_info->ipv4_gateway, goto end);
    CJSON_SET_NUMBER(new_json, "ipv6_enable", network_info->ipv6_enable, goto end);
    CJSON_SET_STRING(new_json, "ipv6_gateway", network_info->ipv6_gateway, goto end);
    CJSON_SET_STRING(new_json, "dns", network_info->dns, goto end);

    *json = new_json;
    return 0;
end:
    cJSON_free(new_json);
    return -1;

}

int StructToCjsonAlgorithm(void* st, cJSON** json) {
    CHECK_POINTER(st, -1);
    CHECK_POINTER(json, return -1);

    Algorithm* algorithm = (Algorithm*)st;
    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, -1);
        
    CJSON_SET_NUMBER(new_json, "detection_enable", algorithm->detection_enable, goto end);
    CJSON_SET_NUMBER(new_json, "tracking_enable", algorithm->tracking_enable, goto end);
    CJSON_SET_NUMBER(new_json, "action_analyze_enable", algorithm->action_analyze_enable, goto end);

    *json = new_json;
    return 0;
end:
    cJSON_free(new_json);
    return -1;
}

int StructToCjsonAlarmInfo(void* st, cJSON** json) {
    CHECK_POINTER(st, -1);

    AlarmInfo* alarm_info = (AlarmInfo*)st;
    cJSON* new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, -1);

    CJSON_SET_NUMBER(new_json, "type", alarm_info->type, goto end);
    CJSON_SET_NUMBER(new_json, "state", alarm_info->state, goto end);
    CJSON_SET_NUMBER(new_json, "time", alarm_info->time, goto end);

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

    CJSON_SET_NUMBER(new_json, "yaw", peripheral_info->yaw, goto end);
    CJSON_SET_NUMBER(new_json, "pitch", peripheral_info->pitch, goto end);
    CJSON_SET_NUMBER(new_json, "motor_temperature", peripheral_info->motor_temperature, goto end);
    CJSON_SET_NUMBER(new_json, "input_voltage", peripheral_info->input_voltage, goto end);
    CJSON_SET_NUMBER(new_json, "working_ampere", peripheral_info->working_ampere, goto end);
    CJSON_SET_NUMBER(new_json, "longitude", peripheral_info->longitude, goto end);
    CJSON_SET_NUMBER(new_json, "latitude", peripheral_info->latitude, goto end);

    *json = new_json;
    return 0;
end:
    cJSON_free(new_json);
    return -1;
}
