#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "network.h"
#include "network_common.h"

typedef struct {
    SystemInfo system_info;
    CameraChipInfo camera_chip_info;
    Position position;
    CalibrationGun calibration_gun;
    PtzInfo ptz_info;
    Areas areas;
    OtherConfig other_config;
    NetworkInfo network_info;
    Algorithm algorithm;
}Mng;
static Mng kMng;

int GetAbility(SupportFunction* func) {
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
    if (ctrl->type == NETWORK_CONTORL_BAD_PIX && ctrl->in.bad_pix_operation == BAD_PIX_OPERATION_GET) {
        ctrl->out.bad_pix_num = 123;
    } else if (ctrl->type == NETWORK_CONTORL_LASER_RANGING && ctrl->in.laser_ranging.mode == LASER_RANGING_SINGLE) {
        ctrl->out.distence = 12345;
    }
    return 0;
}

int Upgrade(const char* path) {
    return 0;
}

int main(int argc, char** argv) {
    NetworkInit("192.168.100.5");

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