#include "cjson_to_struct.h"
#include "cjson_common.h"
#include "check_common.h"

/** 配置 **/
int CjsonToStructSystemInfo(cJSON* json, void* st) {
    CHECK_POINTER(json, return -1);
    CHECK_POINTER(st, return -1);

    SystemInfo* system_info = (SystemInfo*)st;
    CJSON_GET_STRING(json, "language", system_info->language, sizeof(system_info->language), goto end);
    CJSON_GET_STRING(json, "time_zone", system_info->time_zone, sizeof(system_info->time_zone), goto end);
    CJSON_GET_NUMBER(json, "time_format", system_info->time_format, sizeof(system_info->time_format), goto end);
    CJSON_GET_NUMBER(json, "summer_time_enable", system_info->summer_time_enable, sizeof(system_info->summer_time_enable), goto end);
    
    return 0;
end:
    return -1;
}

int CjsonToStructCameraChipInfo(cJSON* json, void* st) {
    CHECK_POINTER(json, return -1);
    CHECK_POINTER(st, return -1);

    CameraChipInfo* camera_chip_info = (CameraChipInfo*)st;
    CJSON_GET_NUMBER(json, "brigthness", camera_chip_info->brigthness, sizeof(camera_chip_info->brigthness), goto end);
    CJSON_GET_NUMBER(json, "contrast", camera_chip_info->contrast, sizeof(camera_chip_info->contrast), goto end);
    CJSON_GET_NUMBER(json, "pseudo_color", camera_chip_info->pseudo_color, sizeof(camera_chip_info->pseudo_color), goto end);
    CJSON_GET_NUMBER(json, "sharpening", camera_chip_info->sharpening, sizeof(camera_chip_info->sharpening), goto end);
    CJSON_GET_NUMBER(json, "central_enhance", camera_chip_info->central_enhance, sizeof(camera_chip_info->central_enhance), goto end);
    CJSON_GET_NUMBER(json, "detail_enhance", camera_chip_info->detail_enhance, sizeof(camera_chip_info->detail_enhance), goto end);
    CJSON_GET_NUMBER(json, "electronic_zoom", camera_chip_info->electronic_zoom, sizeof(camera_chip_info->electronic_zoom), goto end);
    CJSON_GET_NUMBER(json, "target_recognition", camera_chip_info->target_recognition, sizeof(camera_chip_info->target_recognition), goto end);
    CJSON_GET_NUMBER(json, "background_calibration", camera_chip_info->background_calibration, sizeof(camera_chip_info->background_calibration), goto end);
    CJSON_GET_NUMBER(json, "hot_spot_tracking", camera_chip_info->hot_spot_tracking, sizeof(camera_chip_info->hot_spot_tracking), goto end);
    CJSON_GET_NUMBER(json, "picture_in_picture", camera_chip_info->picture_in_picture, sizeof(camera_chip_info->picture_in_picture), goto end);
    CJSON_GET_NUMBER(json, "outdoor_mode", camera_chip_info->outdoor_mode, sizeof(camera_chip_info->outdoor_mode), goto end);
    CJSON_GET_NUMBER(json, "fusion_mod", camera_chip_info->fusion_mod, sizeof(camera_chip_info->fusion_mod), goto end);
    CJSON_GET_NUMBER(json, "shutter_calibration", camera_chip_info->shutter_calibration, sizeof(camera_chip_info->shutter_calibration), goto end);
    CJSON_GET_NUMBER(json, "bad_pix_threshold", camera_chip_info->bad_pix_threshold, sizeof(camera_chip_info->bad_pix_threshold), goto end);
    
    return 0;
end:
    return -1;
}

int CjsonToStructPosition(cJSON* json, void* st) {
    CHECK_POINTER(json, return -1);
    CHECK_POINTER(st, return -1);

    Position* position = (Position*)st;
    CJSON_GET_NUMBER(json, "position_mode", position->position_mode, sizeof(position->position_mode), goto end);
    CJSON_GET_NUMBER(json, "external_position", position->external_position, sizeof(position->external_position), goto end);
    CJSON_GET_NUMBER(json, "target_guidance", position->target_guidance, sizeof(position->target_guidance), goto end);
    CJSON_GET_NUMBER(json, "first_and_last_target", position->first_and_last_target, sizeof(position->first_and_last_target), goto end);
    CJSON_GET_NUMBER(json, "multi_point_position", position->multi_point_position, sizeof(position->multi_point_position), goto end);

    return 0;
end:
    return -1;
}

static int CjsonToStructPoint(cJSON* json, void* st) {
    CHECK_POINTER(json, return -1);
    CHECK_POINTER(st, return -1);

    Point* point = (Point*)st;
    CJSON_GET_NUMBER(json, "x", point->x, sizeof(point->x), goto end);
    CJSON_GET_NUMBER(json, "y", point->y, sizeof(point->y), goto end);

    return 0;
end:
    return -1;
}

int CjsonToStructCalibrationGun(cJSON* json, void* st) {
    CHECK_POINTER(json, return -1);
    CHECK_POINTER(st, return -1);

    CalibrationGun* calibration_gun = (CalibrationGun*)st;
    CJSON_GET_STRING(json, "gun_type", calibration_gun->gun_type, sizeof(calibration_gun->gun_type), goto end);
    CJSON_GET_NUMBER(json, "colgraticule_style", calibration_gun->colgraticule_style, sizeof(calibration_gun->colgraticule_style), goto end);
    CJSON_GET_NUMBER(json, "color", calibration_gun->color, sizeof(calibration_gun->color), goto end);
    CjsonToStructPoint(cJSON_GetObjectItemCaseSensitive(json, "coordinate"), &calibration_gun->coordinate);

    return 0;
end:
    return -1;
}

static int CjsonToStructConstantScan(cJSON* json, void* st) {
    CHECK_POINTER(json, return -1);
    CHECK_POINTER(st, return -1);

    ConstantScan* constant_scan = (ConstantScan*)st;
    CJSON_GET_NUMBER(json, "deflection_mode", constant_scan->deflection_mode, sizeof(constant_scan->deflection_mode), goto end);
    CJSON_GET_NUMBER(json, "yaw", constant_scan->yaw, sizeof(constant_scan->yaw), goto end);
    CjsonToStructPoint(cJSON_GetObjectItemCaseSensitive(json, "pix"), &constant_scan->pix);

    return 0;
end:
    return -1;
}

static int CjsonToStructFanScanning(cJSON* json, void* st) {
    CHECK_POINTER(json, return -1);
    CHECK_POINTER(st, return -1);

    FanScanning* fan_scanning = (FanScanning*)st;
    CJSON_GET_NUMBER(json, "start_angle", fan_scanning->start_angle, sizeof(fan_scanning->start_angle), goto end);
    CJSON_GET_NUMBER(json, "end_angle", fan_scanning->end_angle, sizeof(fan_scanning->end_angle), goto end);

    return 0;
end:
    return -1;
}

int CjsonToStructPtzInfo(cJSON* json, void* st) {
    CHECK_POINTER(json, return -1);
    CHECK_POINTER(st, return -1);

    PtzInfo* ptz_info = (PtzInfo*)st;
    CJSON_GET_NUMBER(json, "motor_enable", ptz_info->motor_enable, sizeof(ptz_info->motor_enable), goto end);
    CJSON_GET_NUMBER(json, "scan_mode", ptz_info->scan_mode, sizeof(ptz_info->scan_mode), goto end);
    CjsonToStructConstantScan(cJSON_GetObjectItemCaseSensitive(json, "constant_scan"), &ptz_info->constant_scan);
    CjsonToStructFanScanning(cJSON_GetObjectItemCaseSensitive(json, "fan_scanning"), &ptz_info->fan_scanning);
    CJSON_GET_NUMBER(json, "pitch", ptz_info->pitch, sizeof(ptz_info->pitch), goto end);
    CJSON_GET_NUMBER(json, "step", ptz_info->step, sizeof(ptz_info->step), goto end);
    CJSON_GET_NUMBER(json, "speed", ptz_info->speed, sizeof(ptz_info->speed), goto end);

    return 0;
end:
    return -1;
}

static int CjsonToStructArea(cJSON* json, void* st) {
    CHECK_POINTER(json, return -1);
    CHECK_POINTER(st, return -1);

    Area* area = (Area*)st;
    CJSON_GET_STRING(json, "area_name", area->area_name, sizeof(area->area_name), goto end);
    CJSON_GET_NUMBER(json, "enable", area->enable, sizeof(area->enable), goto end);
    CJSON_GET_NUMBER(json, "point_num_max", area->point_num_max, sizeof(area->point_num_max), goto end);
    CJSON_GET_NUMBER(json, "point_num", area->point_num, sizeof(area->point_num), goto end);

    cJSON* points = cJSON_GetObjectItemCaseSensitive(json, "point");
    CHECK_POINTER(points, return -1);
    CHECK_BOOL(cJSON_IsArray(points), return -1);

    for(int i = 0; i < area->point_num && i < area->point_num_max; i++) {
        cJSON* point_item = cJSON_GetArrayItem(points, i);
        CHECK_POINTER(point_item, goto end);

        CjsonToStructPoint(point_item, &area->point[i]);
    }

    return 0;
end:
    return -1;
}

int CjsonToStructAreas(cJSON* json, void* st) {
    CHECK_POINTER(json, return -1);
    CHECK_POINTER(st, return -1);

    if (!cJSON_IsArray(json)) {
        LOG_ERR("json is not array!");
        return -1;
    }

    Areas* areas = (Areas*)st;
    areas->area_num = cJSON_GetArraySize(json);
    for(int i = 0; i < areas->area_num && i < NET_AREA_NUM_MAX; i++) {
        cJSON* area_item = cJSON_GetArrayItem(json, i);
        CHECK_POINTER(area_item, goto end);

        CjsonToStructArea(area_item, &areas->area[i]);
    }
    
    return 0;
end:
    return -1;
}

static int CjsonToStructRedDotInfo(cJSON* json, void* st) {
    CHECK_POINTER(json, return -1);
    CHECK_POINTER(st, return -1);

    RedDotInfo* red_dot_info = (RedDotInfo*)st;
    CJSON_GET_NUMBER(json, "deflection_mode", red_dot_info->enable, sizeof(red_dot_info->enable), goto end);
    CjsonToStructPoint(cJSON_GetObjectItemCaseSensitive(json, "coordinate"), &red_dot_info->coordinate);

    return 0;
end:
    return -1;
}

int CjsonToStructOtherConfig(cJSON* json, void* st) {
    CHECK_POINTER(json, return -1);
    CHECK_POINTER(st, return -1);

    OtherConfig* other_conf = (OtherConfig*)st;
    CJSON_GET_NUMBER(json, "defog_by_heat_enable", other_conf->defog_by_heat_enable, sizeof(other_conf->defog_by_heat_enable), goto end);
    CJSON_GET_NUMBER(json, "heat_enable", other_conf->heat_enable, sizeof(other_conf->heat_enable), goto end);
    CJSON_GET_NUMBER(json, "fan_enable", other_conf->fan_enable, sizeof(other_conf->fan_enable), goto end);
    CJSON_GET_NUMBER(json, "electronic_compass_enable", other_conf->electronic_compass_enable, sizeof(other_conf->fanelectronic_compass_enable_enable), goto end);
    CjsonToStructRedDotInfo(cJSON_GetObjectItemCaseSensitive(json, "red_dot"), &other_conf->red_dot);
    CJSON_GET_NUMBER(json, "hdmi_enable", other_conf->hdmi_enable, sizeof(other_conf->hdmi_enable), goto end);
    
    return 0;
end:
    return -1;
}

int CjsonToStructNetworkInfo(cJSON* json, void* st) {
    CHECK_POINTER(json, return -1);
    CHECK_POINTER(st, return -1);

    NetworkInfo* network_info = (NetworkInfo*)st;
    CJSON_GET_NUMBER(json, "dhcp_enable", network_info->dhcp_enable, sizeof(network_info->dhcp_enable), goto end);
    CJSON_GET_NUMBER(json, "ipv4_enable", network_info->ipv4_enable, sizeof(network_info->ipv4_enable), goto end);
    CJSON_GET_STRING(json, "ipv4_addr", network_info->ipv4_addr, sizeof(network_info->ipv4_addr), goto end);
    CJSON_GET_STRING(json, "ipv4_mask", network_info->ipv4_mask, sizeof(network_info->ipv4_mask), goto end);
    CJSON_GET_STRING(json, "ipv4_gateway", network_info->ipv4_gateway, sizeof(network_info->ipv4_gateway), goto end);
    CJSON_GET_NUMBER(json, "ipv6_enable", network_info->ipv6_enable, sizeof(network_info->ipv6_enable), goto end);
    CJSON_GET_STRING(json, "ipv6_gateway", network_info->ipv6_gateway, sizeof(network_info->ipv6_gateway), goto end);
    CJSON_GET_STRING(json, "dns", network_info->dns, sizeof(network_info->dns), goto end);
    
    return 0;
end:
    return -1;
}

int CjsonToStructAlgorithm(cJSON* json, void* st) {
    CHECK_POINTER(json, return -1);
    CHECK_POINTER(st, return -1);

    Algorithm* algorithm = (Algorithm*)st;
    CJSON_GET_NUMBER(json, "detection_enable", algorithm->detection_enable, sizeof(algorithm->detection_enable), goto end);
    CJSON_GET_NUMBER(json, "tracking_enable", algorithm->tracking_enable, sizeof(algorithm->tracking_enable), goto end);
    CJSON_GET_NUMBER(json, "action_analyze_enable", algorithm->action_analyze_enable, sizeof(algorithm->action_analyze_enable), goto end);
    
    return 0;
end:
    return -1;
}

/** 上报 **/
int CjsonToStructAlarmInfo(cJSON* json, void* st) {
    CHECK_POINTER(json, -1);
    CHECK_POINTER(st, -1);

    AlarmInfo* alarm_info = (AlarmInfo*)st;
    CJSON_GET_NUMBER(json, "type", alarm_info->type, sizeof(alarm_info->type), goto end);
    CJSON_GET_NUMBER(json, "state", alarm_info->state, sizeof(alarm_info->state), goto end);
    CJSON_GET_NUMBER(json, "time", alarm_info->time, sizeof(alarm_info->time), goto end);
    
    return 0;
end:
    return -1;
}

int CjsonToStructPeripheralInfo(cJSON* json, void* st) {
    CHECK_POINTER(json, -1);
    CHECK_POINTER(st, -1);

    PeripheralInfo* peripheral_info = (PeripheralInfo*)st;
    CJSON_GET_NUMBER(json, "yaw", peripheral_info->yaw, sizeof(peripheral_info->yaw), goto end);
    CJSON_GET_NUMBER(json, "pitch", peripheral_info->pitch, sizeof(peripheral_info->pitch), goto end);
    CJSON_GET_NUMBER(json, "motor_temperature", peripheral_info->motor_temperature, sizeof(peripheral_info->motor_temperature), goto end);
    CJSON_GET_NUMBER(json, "input_voltage", peripheral_info->input_voltage, sizeof(peripheral_info->input_voltage), goto end);
    CJSON_GET_NUMBER(json, "working_ampere", peripheral_info->working_ampere, sizeof(peripheral_info->working_ampere), goto end);
    CJSON_GET_NUMBER(json, "longitude", peripheral_info->longitude, sizeof(peripheral_info->longitude), goto end);
    CJSON_GET_NUMBER(json, "latitude", peripheral_info->latitude, sizeof(peripheral_info->latitude), goto end);
    
    return 0;
end:
    return -1;
}
