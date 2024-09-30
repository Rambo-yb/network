#include "cjson_to_struct.h"
#include "cjson_common.h"
#include "check_common.h"

/** 配置 **/
int CjsonToStructNetworkSystemInfo(cJSON* json, void* st) {
    CHECK_POINTER(json, return -1);
    CHECK_POINTER(st, return -1);

    NetworkSystemInfo* system_info = (NetworkSystemInfo*)st;
    CJSON_GET_STRING(json, "language", system_info->language, sizeof(system_info->language), goto end);
    CJSON_GET_STRING(json, "time_zone", system_info->time_zone, sizeof(system_info->time_zone), goto end);
    CJSON_GET_NUMBER(json, "time_format", system_info->time_format, sizeof(system_info->time_format), goto end);
    CJSON_GET_NUMBER(json, "summer_time_enable", system_info->summer_time_enable, sizeof(system_info->summer_time_enable), goto end);
    
    return 0;
end:
    return -1;
}

int CjsonToStructNetworkCameraChipInfo(cJSON* json, void* st) {
    CHECK_POINTER(json, return -1);
    CHECK_POINTER(st, return -1);

    NetworkCameraChipInfo* camera_chip_info = (NetworkCameraChipInfo*)st;
    CJSON_GET_NUMBER(json, "brightness", camera_chip_info->brightness, sizeof(camera_chip_info->brightness), goto end);
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

int CjsonToStructNetworkPosition(cJSON* json, void* st) {
    CHECK_POINTER(json, return -1);
    CHECK_POINTER(st, return -1);

    NetworkPosition* position = (NetworkPosition*)st;
    CJSON_GET_NUMBER(json, "position_mode", position->position_mode, sizeof(position->position_mode), goto end);
    CJSON_GET_NUMBER(json, "external_position", position->external_position, sizeof(position->external_position), goto end);
    CJSON_GET_NUMBER(json, "target_guidance", position->target_guidance, sizeof(position->target_guidance), goto end);
    CJSON_GET_NUMBER(json, "first_and_last_target", position->first_and_last_target, sizeof(position->first_and_last_target), goto end);
    CJSON_GET_NUMBER(json, "multi_point_position", position->multi_point_position, sizeof(position->multi_point_position), goto end);

    return 0;
end:
    return -1;
}

static int CjsonToStructNetworkPoint(cJSON* json, void* st) {
    CHECK_POINTER(json, return -1);
    CHECK_POINTER(st, return -1);

    NetworkPoint* point = (NetworkPoint*)st;
    CJSON_GET_NUMBER(json, "x", point->x, sizeof(point->x), goto end);
    CJSON_GET_NUMBER(json, "y", point->y, sizeof(point->y), goto end);

    return 0;
end:
    return -1;
}

int CjsonToStructNetworkCalibrationGun(cJSON* json, void* st) {
    CHECK_POINTER(json, return -1);
    CHECK_POINTER(st, return -1);

    NetworkCalibrationGun* calibration_gun = (NetworkCalibrationGun*)st;
    CJSON_GET_STRING(json, "gun_type", calibration_gun->gun_type, sizeof(calibration_gun->gun_type), goto end);
    CJSON_GET_NUMBER(json, "colgraticule_style", calibration_gun->colgraticule_style, sizeof(calibration_gun->colgraticule_style), goto end);
    CJSON_GET_NUMBER(json, "color", calibration_gun->color, sizeof(calibration_gun->color), goto end);
    CjsonToStructNetworkPoint(cJSON_GetObjectItemCaseSensitive(json, "coordinate"), &calibration_gun->coordinate);

    return 0;
end:
    return -1;
}

static int CjsonToStructNetworkConstantScan(cJSON* json, void* st) {
    CHECK_POINTER(json, return -1);
    CHECK_POINTER(st, return -1);

    NetworkConstantScan* constant_scan = (NetworkConstantScan*)st;
    CJSON_GET_NUMBER(json, "deflection_mode", constant_scan->deflection_mode, sizeof(constant_scan->deflection_mode), goto end);
    CJSON_GET_NUMBER(json, "yaw", constant_scan->yaw, sizeof(constant_scan->yaw), goto end);
    CjsonToStructNetworkPoint(cJSON_GetObjectItemCaseSensitive(json, "pix"), &constant_scan->pix);

    return 0;
end:
    return -1;
}

static int CjsonToStructNetworkFanScanning(cJSON* json, void* st) {
    CHECK_POINTER(json, return -1);
    CHECK_POINTER(st, return -1);

    NetworkFanScanning* fan_scanning = (NetworkFanScanning*)st;
    CJSON_GET_NUMBER(json, "start_angle", fan_scanning->start_angle, sizeof(fan_scanning->start_angle), goto end);
    CJSON_GET_NUMBER(json, "end_angle", fan_scanning->end_angle, sizeof(fan_scanning->end_angle), goto end);

    return 0;
end:
    return -1;
}

int CjsonToStructNetworkPtzInfo(cJSON* json, void* st) {
    CHECK_POINTER(json, return -1);
    CHECK_POINTER(st, return -1);

    NetworkPtzInfo* ptz_info = (NetworkPtzInfo*)st;
    CJSON_GET_NUMBER(json, "motor_enable", ptz_info->motor_enable, sizeof(ptz_info->motor_enable), goto end);
    CJSON_GET_NUMBER(json, "scan_mode", ptz_info->scan_mode, sizeof(ptz_info->scan_mode), goto end);
    CjsonToStructNetworkConstantScan(cJSON_GetObjectItemCaseSensitive(json, "constant_scan"), &ptz_info->constant_scan);
    CjsonToStructNetworkFanScanning(cJSON_GetObjectItemCaseSensitive(json, "fan_scanning"), &ptz_info->fan_scanning);
    CJSON_GET_NUMBER(json, "pitch", ptz_info->pitch, sizeof(ptz_info->pitch), goto end);
    CJSON_GET_NUMBER(json, "step", ptz_info->step, sizeof(ptz_info->step), goto end);
    CJSON_GET_NUMBER(json, "speed", ptz_info->speed, sizeof(ptz_info->speed), goto end);

    return 0;
end:
    return -1;
}

static int CjsonToStructNetworkArea(cJSON* json, void* st) {
    CHECK_POINTER(json, return -1);
    CHECK_POINTER(st, return -1);

    NetworkArea* area = (NetworkArea*)st;
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

        CjsonToStructNetworkPoint(point_item, &area->point[i]);
    }

    return 0;
end:
    return -1;
}

int CjsonToStructNetworkAreas(cJSON* json, void* st) {
    CHECK_POINTER(json, return -1);
    CHECK_POINTER(st, return -1);

    if (!cJSON_IsArray(json)) {
        LOG_ERR("json is not array!");
        return -1;
    }

    NetworkAreas* areas = (NetworkAreas*)st;
    areas->area_num = cJSON_GetArraySize(json);
    for(int i = 0; i < areas->area_num && i < NET_AREA_NUM_MAX; i++) {
        cJSON* area_item = cJSON_GetArrayItem(json, i);
        CHECK_POINTER(area_item, goto end);

        CjsonToStructNetworkArea(area_item, &areas->area[i]);
    }
    
    return 0;
end:
    return -1;
}

static int CjsonToStructNetworkRedDotInfo(cJSON* json, void* st) {
    CHECK_POINTER(json, return -1);
    CHECK_POINTER(st, return -1);

    NetworkRedDotInfo* red_dot_info = (NetworkRedDotInfo*)st;
    CJSON_GET_NUMBER(json, "deflection_mode", red_dot_info->enable, sizeof(red_dot_info->enable), goto end);
    CjsonToStructNetworkPoint(cJSON_GetObjectItemCaseSensitive(json, "coordinate"), &red_dot_info->coordinate);

    return 0;
end:
    return -1;
}

int CjsonToStructNetworkOtherConfig(cJSON* json, void* st) {
    CHECK_POINTER(json, return -1);
    CHECK_POINTER(st, return -1);

    NetworkOtherConfig* other_conf = (NetworkOtherConfig*)st;
    CJSON_GET_NUMBER(json, "defog_by_heat_enable", other_conf->defog_by_heat_enable, sizeof(other_conf->defog_by_heat_enable), goto end);
    CJSON_GET_NUMBER(json, "heat_enable", other_conf->heat_enable, sizeof(other_conf->heat_enable), goto end);
    CJSON_GET_NUMBER(json, "fan_enable", other_conf->fan_enable, sizeof(other_conf->fan_enable), goto end);
    CJSON_GET_NUMBER(json, "electronic_compass_enable", other_conf->electronic_compass_enable, sizeof(other_conf->fanelectronic_compass_enable_enable), goto end);
    CjsonToStructNetworkRedDotInfo(cJSON_GetObjectItemCaseSensitive(json, "red_dot"), &other_conf->red_dot);
    CJSON_GET_NUMBER(json, "hdmi_enable", other_conf->hdmi_enable, sizeof(other_conf->hdmi_enable), goto end);
    
    return 0;
end:
    return -1;
}

int CjsonToStructNetworkNetworkInfo(cJSON* json, void* st) {
    CHECK_POINTER(json, return -1);
    CHECK_POINTER(st, return -1);

    NetworkNetworkInfo* network_info = (NetworkNetworkInfo*)st;
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

int CjsonToStructNetworkAlgorithm(cJSON* json, void* st) {
    CHECK_POINTER(json, return -1);
    CHECK_POINTER(st, return -1);

    NetworkAlgorithm* algorithm = (NetworkAlgorithm*)st;
    CJSON_GET_NUMBER(json, "detection_enable", algorithm->detection_enable, sizeof(algorithm->detection_enable), goto end);
    CJSON_GET_NUMBER(json, "tracking_enable", algorithm->tracking_enable, sizeof(algorithm->tracking_enable), goto end);
    CJSON_GET_NUMBER(json, "action_analyze_enable", algorithm->action_analyze_enable, sizeof(algorithm->action_analyze_enable), goto end);
    
    return 0;
end:
    return -1;
}

/** 上报 **/
int CjsonToStructNetworkAlarmInfo(cJSON* json, void* st) {
    CHECK_POINTER(json, -1);
    CHECK_POINTER(st, -1);

    NetworkAlarmInfo* alarm_info = (NetworkAlarmInfo*)st;
    CJSON_GET_NUMBER(json, "type", alarm_info->type, sizeof(alarm_info->type), goto end);
    CJSON_GET_NUMBER(json, "state", alarm_info->state, sizeof(alarm_info->state), goto end);
    CJSON_GET_NUMBER(json, "time", alarm_info->time, sizeof(alarm_info->time), goto end);
    
    return 0;
end:
    return -1;
}

int CjsonToStructNetworkPeripheralInfo(cJSON* json, void* st) {
    CHECK_POINTER(json, -1);
    CHECK_POINTER(st, -1);

    NetworkPeripheralInfo* peripheral_info = (NetworkPeripheralInfo*)st;
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
