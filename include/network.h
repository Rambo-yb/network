#ifndef __NETWORK_H__
#define __NETWORK_H__

#include "network_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*NetworkOperationGetAbilityCb)(SupportFunction* );

typedef enum {
    NETWORK_SYSTEM_REBOOT = 100,          // 重启
    NETWORK_SYSTEM_RESET,           // 恢复出厂
    NETWORK_SYSTEM_FORMAT,          // 格式化
    NETWORK_SYSTEM_DEVICE_INFO,     // 设备信息
    NETWORK_SYSTEM_SET_TIME,        // 设置时间
    NETWORK_SYSTEM_GET_RTSP_URL,    // 获取rtsp_url
}NetworkSystemType;
typedef struct {
    int type;
    union {
        int time;                   // 设置的UTC时间
    } in;

    union {
        DeviceInfo device_info;     // 获取的设备信息
        char rtsp_url[128];         // 获取的rtsp地址
    } out;
}NetworkSystem;
typedef int (*NetworkOperationSystemCb)(NetworkSystem* );

typedef enum {
    NETWORK_CONFIG_SYSTEM_INFO = 200,         // 系统信息
    NETWORK_CONFIG_CAMERA_CHIP_INFO,    // 机芯信息
    NETWORK_CONFIG_POSITION,            // 定位
    NETWORK_CONFIG_CALIBRATION_GUN,     // 校枪
    NETWORK_CONFIG_PTZ_INFO,            // 云台
    NETWORK_CONFIG_AREAS,           // 区域
    NETWORK_CONFIG_OTHER_INFO,        // 其他信息
    NETWORK_CONFIG_NETWORK,             // 网络信息
    NETWORK_CONFIG_ALGORITHM,           // 算法
}NetworkConfigType;
typedef int (*NetworkOperationGetConfigCb)(NetworkConfigType , void* , int );
typedef int (*NetworkOperationSetConfigCb)(NetworkConfigType , void* , int );

typedef enum {
    NETWORK_CONTORL_SNAP = 300,             // 抓拍
    NETWORK_CONTORL_RECORD,                 // 录像
    NETWORK_CONTORL_BAD_PIX,                // 坏点
    NETWORK_CONTORL_SHUTTER_CALIBRATION,    // 快门校正
    NETWORK_CONTORL_LASER_RANGING,          // 测距
    NETWORK_CONTORL_PTZ,                    // 云台(预留)
    NETWORK_CONTORL_TRACKING,               // 跟踪
}NetworkContorlType;

typedef struct {
    int type;   // enum NetworkContorlType
    union {
        int recode_state;           // 录像状态，0结束，1开始
        int bad_pix_operation;      // enum BadPixOperationType
        LaserRanging laser_ranging;
        TrackingObject tracking_object;
    } in;
    
    union {
        int bad_pix_num;    // 获取坏点数
        int distence;       // 单次测距
    } out;
}NetworkContorl;
typedef int (*NetworkOperationControlCb)(NetworkContorl* );

typedef int (*NetworkOperationUpgradeCb)(const char* );

typedef enum {
    NETWORK_OPERATION_GET_ABILITY,
    NETWORK_OPERATION_SYSTEM_REQUEST,
    NETWORK_OPERATION_GET_CONFIG,
    NETWORK_OPERATION_SET_CONFIG,
    NETWORK_OPERATION_CONTORL_REQUEST,
    NETWORK_OPERATION_UPGRADE,
}NetworkOperationType;
/**
 * @brief 回调注册接口
 */
void NetworkOperationRegister(NetworkOperationType type, void* cb);

/**
 * @brief 网络初始化
 * @return 成功返回 0
 *         失败返回 其他值
*/
int NetworkInit(char* addr, char* log_path);

/**
 * @brief 网络反初始化
*/
void NetworkUnInit();

/**
 * @brief 信息上报函数
 */
int NetworkUploadInfo(NetworkUpload* upload_info);

#ifdef __cplusplus
};
#endif

#endif