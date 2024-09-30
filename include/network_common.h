#ifndef __NETWORK_COMMON_H__
#define __NETWORK_COMMON_H__

#include <stdbool.h>
#include <stdint.h>

/** 获取设备能力 **/
typedef struct {
    bool support_state; // 支持状态
    int min;            // 最小值
    int max;            // 最大值
}NetworkFunctionSoap;

typedef struct {
    uint64_t language_support_mask; // 语言掩码
    bool position_function_support; // 定位功能
    bool ptz_function_support;      // 云台功能
    bool area_function_support;     // 区域功能
    bool onvif_protocol_support;    // onvif协议
    bool ipv6_protocol_support;     // ipv6协议
}NetworkSystemFunction;

typedef struct {
    int brightness_support;                     // 亮度
    int contrast_support;                       // 对比度
    uint64_t pseudo_color_support_mask;         // 极性/伪彩掩码
    int sharpening_support;                     // 锐度
    bool central_enhance_support;               // 中心增强
    int detail_enhance_support;                 // 细节增强
    int electronic_zoom_support;                // 电子变倍，【0：不支持】【1：1/2/4】【2：1-4(±0.1)】【3：1-8(±1)】
    bool target_recognition_support;            // 目标识别
    bool background_calibration_support;        // 背景校正
    bool hot_spot_tracking_support;             // 热点追踪
    uint64_t picture_in_picture_support_mask;   // 画中画掩码
    bool outdoor_mode_support;                  // 户外模式
    uint64_t fusion_mode_support_mask;          // 融合模式
    bool shutter_calibration_support;           // 快门校正
    NetworkFunctionSoap bad_pix_threshold_support;     // 坏点阈值
}NetworkCameraChipFunction;

typedef struct {
    bool support_state;                     // 支持状态
    bool gun_type_support;                  // 枪型
    uint64_t graticule_style_support_mask;  // 分划掩码
    uint64_t color_support_mask;            // 分划颜色掩码
}NetworkCalibrationGunFunction;

typedef struct {
    bool defog_by_heat_support;         // 去雾加热
    bool heat_support;                  // 加热
    bool fan_support;                   // 风扇
    bool electronic_compass_support;    // 电子罗盘
    bool red_dot_support;               // 红点
    bool hdmi_support;                  // hdmi
}NetworkOtherFunction;

typedef struct {
    bool object_detection_support;      // 目标检测
    bool tracking_support;              // 追踪
    bool action_analyze_support;        // 行为分析
    bool splicing_support;              // 拼接
    bool fire_point_support;            // 火点检测
}NetworkAlgorithmFunction;

typedef struct {
    NetworkSystemFunction system_function;
    NetworkCameraChipFunction camera_chip_function;
    NetworkCalibrationGunFunction calibration_gun_function;
    NetworkOtherFunction other_function;
    NetworkAlgorithmFunction algorithm_function;
}NetworkSupportFunction;

/** 系统请求 **/
typedef struct {
    char soft_version[32];  // 软件版本
    char hard_version[32];  // 硬件版本
    char serial_number[32]; // 序列号
}NetworkDeviceInfo;

/** 配置 **/
typedef struct {
    int x;
    int y;
}NetworkPoint;

typedef struct {
    char language[32];          // 语言
    char time_zone[16];         // 时区
    int time_format;            // 时间格式
    bool summer_time_enable;    // 夏令时使能
}NetworkSystemInfo;

typedef enum {
    NETWORK_PSEUDO_COLOR_WHITE_HOT,     // 白热
    NETWORK_PSEUDO_COLOR_BLACK_HOT,     // 黑热
    NETWORK_PSEUDO_COLOR_IRON_RED,      // 铁红
    NETWORK_PSEUDO_COLOR_DESERT_YELLOW, // 沙漠黄
    NETWORK_PSEUDO_COLOR_GREEN_HOT,     // 绿热
    NETWORK_PSEUDO_COLOR_RED_HOT,       // 红热
    NETWORK_PSEUDO_COLOR_SKY,           // 天空
    NETWORK_PSEUDO_COLOR_EDGE,          // 边缘
    NETWORK_PSEUDO_COLOR_RAINBOW,       // 彩虹
}NetworkPseudoColor;

typedef enum {
    NETWORK_PICTURE_IN_PICTURE_OFF,         // 关闭
    NETWORK_PICTURE_IN_PICTURE_LEFT_UP,     // 左上
    NETWORK_PICTURE_IN_PICTURE_MIDDLE_UP,   // 中上
    NETWORK_PICTURE_IN_PICTURE_REIGHT_UP,   // 右上
}NetworkPictureInPicturePos;

typedef enum {
    NETWORK_FUSION_VISIBLE_LIGHT,               // 可见光
    NETWORK_FUSION_INFRARED,                    // 红外
    NETWORK_FUSION_VISIBLE_LIGHT_AND_INFRARED,  // 融合
}NetworkFusionMode;

typedef struct {
    int brightness;                 // 亮度
    int contrast;                   // 对比度
    int pseudo_color;               // 极性/伪彩，enum NetworkPseudoColor
    int sharpening;                 // 锐化
    bool central_enhance;           // 中心增强
    int detail_enhance;             // 细节增强
    float electronic_zoom;          // 电子变倍
    bool target_recognition;        // 目标识别
    bool background_calibration;    // 背景校正
    bool hot_spot_tracking;         // 红点追踪
    int picture_in_picture;         // 画中画，enum NetworkPictureInPicturePos
    bool outdoor_mode;              // 户外模式
    int fusion_mod;                 // 融合，enum NetworkFusionMode
    bool shutter_calibration;       // 快门校正
    int bad_pix_threshold;          // 坏点阈值
}NetworkCameraChipInfo;

typedef struct {
    int position_mode;              // 定位模式
    bool external_position;         // 外部定位
    bool target_guidance;           // 目标引导
    bool first_and_last_target;     // 首末目标
    bool multi_point_position;      // 多点定位
}NetworkPosition;

typedef enum {
    NETWORK_COLGRATICULE_COLOR_WHITE,   // 白
    NETWORK_COLGRATICULE_COLOR_RED,     // 红
    NETWORK_COLGRATICULE_COLOR_YELLOW,  // 黄
    NETWORK_COLGRATICULE_COLOR_BLUE,    // 蓝
}NetworkColgraticuleColor;

typedef struct {
    char gun_type[32];          // 枪型
    int colgraticule_style;     // 分划样式 
    int color;                  // 颜色，enum NetworkColgraticuleColor
    NetworkPoint coordinate;           // 坐标
}NetworkCalibrationGun;

typedef enum {
    NETWORK_CONSTAN_SCAN_DEFLECTION_ANGLE,  // 角度
    NETWORK_CONSTAN_SCAN_DEFLECTION_PIX,    // 像素点
}NetworkConstanScanDeflectionMode;

typedef struct {
    int deflection_mode;    // 偏转模式，enum NetworkConstanScanDeflectionMode
    double yaw;             // 角度
    NetworkPoint pix;              // 像素点
}NetworkConstantScan;

typedef struct {
    double start_angle;     // 起始角
    double end_angle;       // 终止角
}NetworkFanScanning;

typedef enum {
    NETWORK_PTZ_SCAN_WEEKLY,    // 周扫描
    NETWORK_PTZ_SCAN_FAN,       // 扇扫描
    NETWORK_PTZ_SCAN_CONSTANT,  // 定扫描
    NETWORK_PTZ_SCAN_CONTROL,   // 控制
}NetworkPtzScanMode;

typedef struct {
    bool motor_enable;          // 电机使能  
    int scan_mode;              // 扫描模式，enum NetworkPtzScanMode
    NetworkConstantScan constant_scan; 
    NetworkFanScanning fan_scanning;
    double pitch;               // 俯仰角
    double step;                // 步长
    int speed;                  // 速度
}NetworkPtzInfo;

#define NET_AREA_NAME_SIAE (128)
#define NET_POINT_NUM_MAX (8)
#define NET_AREA_NUM_MAX (32)

typedef struct {
    char area_name[NET_AREA_NAME_SIAE]; // 区域名
    bool enable;                        // 区域使能
    int point_num_max;                  // 区域点最大个数
    int point_num;                      // 区域点数
    NetworkPoint point[NET_POINT_NUM_MAX];     // 区域点坐标
}NetworkArea;

typedef struct {
    int area_num;
    NetworkArea area[NET_AREA_NUM_MAX];
}NetworkAreas;

typedef struct {
    bool enable;        // 使能
    NetworkPoint coordinate;   // 坐标
}NetworkRedDotInfo;

typedef struct {
    bool defog_by_heat_enable;          // 去雾加热
    bool heat_enable;                   // 加热
    bool fan_enable;                    // 风扇
    bool electronic_compass_enable;     // 电子罗盘
    NetworkRedDotInfo red_dot;          // 红点
    bool hdmi_enable;                   // hdmi
}NetworkOtherConfig;

typedef struct {
    bool dhcp_enable;
    bool ipv4_enable;
    char ipv4_addr[16];
    char ipv4_mask[16];
    char ipv4_gateway[16];
    bool ipv6_enable;
    char ipv6_addr[128];
    char ipv6_gateway[128];
    char dns[16];
}NetworkNetworkInfo;

typedef struct {
    bool detection_enable;          // 目标检测
    bool tracking_enable;           // 追踪
    bool action_analyze_enable;     // 行为分析
}NetworkAlgorithm;

/** 控制请求 **/
typedef enum {
    NETWORK_LASER_RANGING_SINGLE,       // 单次
    NETWORK_LASER_RANGING_CONTINUED,    // 持续
}NetworkLaserRangingMode;

typedef struct {
    int mode;   // enum NetworkLaserRangingMode   
    int state;  // 持续测距，0停止，1开始
}NetworkLaserRanging;

typedef enum {
    NETWORK_PTZ_CTRL_UP,
    NETWORK_PTZ_CTRL_DOWN,
    NETWORK_PTZ_CTRL_LEFT,
    NETWORK_PTZ_CTRL_RIGHT,
    NETWORK_PTZ_CTRL_HOME,
    NETWORK_PTZ_CTRL_PRESET, // 转到预置点，回调携带PRESET_NAME
}NetworkPtzCtrlMode;

typedef struct {
    int mode;   // enum NetworkPtzCtrlMode   
    char preset_name[64];  // 预置点名
}NetworkPtzCtrl;

#define NET_TRACKING_OBJECT_MAX_NUM (16)
typedef struct {
    int tracking_num;
    int tracking_object[NET_TRACKING_OBJECT_MAX_NUM];
}NetworkTrackingObject;

typedef enum {
    NETWORK_BAD_PIX_OPERATION_GET,      // 获取
    NETWORK_BAD_PIX_OPERATION_DELETE,   // 清除
    NETWORK_BAD_PIX_OPERATION_RESET,    // 恢复
    NETWORK_BAD_PIX_OPERATION_SAVE,     // 保存
}NetworkBadPixOperationType;

/** 设备上报 **/
typedef enum {
    NETWORK_ALARM_TYPE_OVER_BOUNDARY,            // 越界  
    NETWORK_ALARM_TYPE_AREA_INTRUDE,             // 区域入侵
    NETWORK_ALARM_TYPE_ABNORMAL_ACTION,          // 异常行为
    NETWORK_ALARM_TYPE_FIRE_POINT_DETECTION,     // 火点检测
    NETWORK_ALARM_TYPE_TEMPERATURE_ABNORMAL,     // 温度异常
    NETWORK_ALARM_TYPE_VOLTAGE_ABNORMAL,         // 电压异常
    NETWORK_ALARM_TYPE_AMPERE_ABNORMAL,          // 电流异常
}NetworkAlarmType;

typedef enum {
    NETWORK_ALARM_STATE_START,      // 开始
    NETWORK_ALARM_STATE_STOP,       // 停止
    NETWORK_ALARM_STATE_TRIGGER,    // 触发
}NetworkAlarmState;

typedef struct NetworkAlarmInfo {
    int type;       // 报警类型，enum NetworkAlarmType
    int state;      // 报警状态，enum NetworkAlarmState
    uint32_t time;  // 时间
}NetworkAlarmInfo;

typedef struct {
    double yaw;                 // 航偏角
    double pitch;               // 俯仰角
    double motor_temperature;   // 电机温度
    double input_voltage;       // 输入电压
    double working_ampere;      // 工作电流
    double longitude;           // 经度
    double latitude;            // 纬度
}NetworkPeripheralInfo;

#define NETWORK_UPLOAD_DOWNLOADING "downloading"    // 下载中
#define NETWORK_UPLOAD_DOWNLOAD_SUCCESS "download_success"  // 下载成功
#define NETWORK_UPLOAD_DOWNLOAD_FAIL "download_fail"    // 下载失败

#define NETWORK_UPLOAD_UPGRADING "upgrading"    // 升级中
#define NETWORK_UPLOAD_UPGRADE_SUCCESS "upgrade_success"    // 升级成功
#define NETWORK_UPLOAD_UPGRADE_FAIL "upgrade_fail"  // 升级失败

typedef enum {
    NETWORK_UPLOAD_ALARM = 400,           // 报警
    NETWORK_UPLOAD_PERIPHERAL,      // 外设
    NETWORK_UPLOAD_DOWNLOAD,         // 下载
    NETWORK_UPLOAD_UPGRADE,         // 升级
    NETWORK_UPLOAD_LASER_RANGING,   // 测距
}NetworkUploadType;

typedef struct {
    int type; // enum NetworkUploadType
    union {
        NetworkAlarmInfo alarm_info;
        NetworkPeripheralInfo peripherail_info;
        char download_state[32];  // NETWORK_UPLOAD_DOWNLOADXXX
        char upgrade_state[32];  // NETWORK_UPLOAD_UPGRADXXX
        int distence;
    } data;
}NetworkUpload;

#endif