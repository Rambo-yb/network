#ifndef __NETWORK_COMMON_H__
#define __NETWORK_COMMON_H__

#include <stdbool.h>

#define NET_AREA_NAME_SIAE (128)
#define NET_POINT_NUM_MAX (8)
#define NET_AREA_NUM_MAX (32)
#define NET_TRACK_ID_NUM_MAX (128)

typedef struct Point {
    int x;
    int y;
}Point;

typedef struct Area {
    char area_name[NET_AREA_NAME_SIAE];
    bool enable;
    int point_num_max;
    int point_num;
    Point point[NET_POINT_NUM_MAX];
}Area;

typedef struct Areas {
    int area_num;
    Area area[NET_AREA_NUM_MAX];
}Areas;


typedef struct ConstantScan {
    int value_type;
    double yaw;
    Point pix;
}ConstantScan;

typedef struct FanScanning {
    double start_angle;
    double end_angle;
}FanScanning;

typedef struct PtzCtrl {
    bool motor_enable;   
    int scan_mode;
    ConstantScan constant_scan;
    FanScanning fan_scanning;
    double pitch;
    double step; 
    int speed;
    int zero_falg;
}PtzCtrl;


typedef struct ChipCtrl {
    int contrast;
    int brightness;
    int polarity;
    int bad_spot_threshold;
    int bad_spot_oper;
    bool hot_spot_track;
}ChipCtrl;


typedef struct OtherCtrl {
    bool defog_by_heat_enable;
    bool heat_enable;
    bool fan_enable;
}OtherCtrl;


typedef struct TrackingObject {
    int id_num;
    int id[NET_TRACK_ID_NUM_MAX];
}TrackingObject;

typedef struct AlgorithmEnable {
    bool detection_enable;
    bool tracking_enable;
    bool action_analyze_enable;
    TrackingObject tracking_object;
}AlgorithmEnable;




typedef struct AlarmInfo {
    bool over_boundary;
    bool area_intrude;
    bool abnormal_action;
    bool fire_point_detection;
    bool temperature_abnormal;
    bool voltage_abnormal;
    bool ampere_abnormal;
}AlarmInfo;

typedef struct PeripheralInfo {
    double yaw;
    double pitch;
    double motor_temperature;
    double input_voltage;
    double working_ampere;
    double longitude;
    double latitude;
}PeripheralInfo;

#endif