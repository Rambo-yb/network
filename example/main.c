#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "network.h"
#include "network_common.h"

typedef struct {
    NetworkSystemInfo system_info;
    NetworkCameraChipInfo camera_chip_info;
    NetworkPosition position;
    NetworkCalibrationGun calibration_gun;
    NetworkPtzInfo ptz_info;
    NetworkAreas areas;
    NetworkOtherConfig other_config;
    NetworkNetworkInfo network_info;
    NetworkAlgorithm algorithm;
}Mng;
static Mng kMng;

int GetAbility(NetworkSupportFunction* func) {
    func->system_function.language_support_mask = 0;
    func->system_function.position_function_support = false;
    func->system_function.ptz_function_support = true;
    func->system_function.area_function_support = true;
    func->system_function.onvif_protocol_support = true;
    func->system_function.ipv6_protocol_support =false;
    
    func->camera_chip_function.brightness_support = 99;
    func->camera_chip_function.contrast_support = 99;
    func->camera_chip_function.pseudo_color_support_mask = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 < 7);
    func->camera_chip_function.sharpening_support = 7;
    func->camera_chip_function.central_enhance_support = false;
    func->camera_chip_function.detail_enhance_support = 0;
    func->camera_chip_function.electronic_zoom_support = 0;
    func->camera_chip_function.target_recognition_support = false;
    func->camera_chip_function.background_calibration_support = false;
    func->camera_chip_function.hot_spot_tracking_support = true;
    func->camera_chip_function.picture_in_picture_support_mask = 0;
    func->camera_chip_function.outdoor_mode_support = false;
    func->camera_chip_function.fusion_mode_support_mask = false;
    func->camera_chip_function.shutter_calibration_support = false;
    func->camera_chip_function.bad_pix_threshold_support.support_state = true;
    func->camera_chip_function.bad_pix_threshold_support.min = 20;
    func->camera_chip_function.bad_pix_threshold_support.max = 32;
    
    func->calibration_gun_function.support_state = false;
    func->calibration_gun_function.gun_type_support = false;
    func->calibration_gun_function.graticule_style_support_mask = 0;
    func->calibration_gun_function.color_support_mask = 0;
    
    func->other_function.defog_by_heat_support = true;
    func->other_function.heat_support = true;
    func->other_function.fan_support = true;
    func->other_function.electronic_compass_support = false;
    func->other_function.red_dot_support = false;
    func->other_function.hdmi_support = false;

    func->algorithm_function.object_detection_support = true;
    func->algorithm_function.tracking_support = true;
    func->algorithm_function.action_analyze_support = false;
    func->algorithm_function.splicing_support = true;
    func->algorithm_function.fire_point_support = false;
    return 0;
}

int SystemRequest(NetworkSystem* sys) {
    printf("system request type:%d\n", sys->type);
    if (sys->type == NETWORK_SYSTEM_DEVICE_INFO) {
        snprintf(sys->out.device_info.soft_version, sizeof(sys->out.device_info.soft_version), "V01.10");
        snprintf(sys->out.device_info.hard_version, sizeof(sys->out.device_info.hard_version), "V00.01");
        snprintf(sys->out.device_info.serial_number, sizeof(sys->out.device_info.serial_number), "1357924680");
    } else if (sys->type == NETWORK_SYSTEM_GET_RTSP_URL) {
        snprintf(sys->out.rtsp_url, sizeof(sys->out.rtsp_url), "rtsp://192.168.100.5/live/main_stream");
    }
    return 0;
}

int SetConfig(NetworkConfigType type, void* conf, int size) {
    switch (type)
    {
    case NETWORK_CONFIG_SYSTEM_INFO:
        memcpy(&kMng.system_info, conf, size);
        break;
    case NETWORK_CONFIG_CAMERA_CHIP_INFO:
        memcpy(&kMng.camera_chip_info, conf, size);
        break;
    case NETWORK_CONFIG_POSITION:
        memcpy(&kMng.position, conf, size);
        break;
    case NETWORK_CONFIG_CALIBRATION_GUN:
        memcpy(&kMng.calibration_gun, conf, size);
        break;
    case NETWORK_CONFIG_PTZ_INFO:
        memcpy(&kMng.ptz_info, conf, size);
        break;
    case NETWORK_CONFIG_AREAS:
        memcpy(&kMng.areas, conf, size);
        break;
    case NETWORK_CONFIG_OTHER_INFO:
        memcpy(&kMng.other_config, conf, size);
        break;
    case NETWORK_CONFIG_NETWORK:
        memcpy(&kMng.network_info, conf, size);
        break;
    case NETWORK_CONFIG_ALGORITHM:
        memcpy(&kMng.algorithm, conf, size);
        break;
    default:
        break;
    }
    return 0;
}

int GetConfig(NetworkConfigType type, void* conf, int size) {
    switch (type)
    {
    case NETWORK_CONFIG_SYSTEM_INFO:
        memcpy(conf, &kMng.system_info, size);
        break;
    case NETWORK_CONFIG_CAMERA_CHIP_INFO:
        memcpy(conf, &kMng.camera_chip_info, size);
        break;
    case NETWORK_CONFIG_POSITION:
        memcpy(conf, &kMng.position, size);
        break;
    case NETWORK_CONFIG_CALIBRATION_GUN:
        memcpy(conf, &kMng.calibration_gun, size);
        break;
    case NETWORK_CONFIG_PTZ_INFO:
        memcpy(conf, &kMng.ptz_info, size);
        break;
    case NETWORK_CONFIG_AREAS:
        memcpy(conf, &kMng.areas, size);
        break;
    case NETWORK_CONFIG_OTHER_INFO:
        memcpy(conf, &kMng.other_config, size);
        break;
    case NETWORK_CONFIG_NETWORK:
        memcpy(conf, &kMng.network_info, size);
        break;
    case NETWORK_CONFIG_ALGORITHM:
        memcpy(conf, &kMng.algorithm, size);
        break;
    default:
        break;
    }
    return 0;
}

int ControlRequest(NetworkContorl* ctrl) {
    printf("control request type:%d\n", ctrl->type);
    if (ctrl->type == NETWORK_CONTORL_BAD_PIX && ctrl->in.bad_pix_operation == NETWORK_BAD_PIX_OPERATION_GET) {
        ctrl->out.bad_pix_num = 123;
    } else if (ctrl->type == NETWORK_CONTORL_LASER_RANGING && ctrl->in.laser_ranging.mode == NETWORK_LASER_RANGING_SINGLE) {
        ctrl->out.distence = 12345;
    } else if (ctrl->type == NETWORK_CONTORL_PTZ) {
        printf("ptz ctrl mode :%d\n", ctrl->in.ptz_ctrl.mode);
    }
    return 0;
}

int Upgrade(const char* path) {
    return 0;
}

int main(int argc, char** argv) {
    NetworkInit("192.168.110.223");

    NetworkOperationRegister(NETWORK_OPERATION_GET_ABILITY, GetAbility);
    NetworkOperationRegister(NETWORK_OPERATION_SYSTEM_REQUEST, SystemRequest);
    NetworkOperationRegister(NETWORK_OPERATION_GET_CONFIG, GetConfig);
    NetworkOperationRegister(NETWORK_OPERATION_SET_CONFIG, SetConfig);
    NetworkOperationRegister(NETWORK_OPERATION_CONTORL_REQUEST, ControlRequest);
    NetworkOperationRegister(NETWORK_OPERATION_UPGRADE, Upgrade);

    while (1) {
        sleep(1);
    }

}