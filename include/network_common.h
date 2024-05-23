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

typedef struct PtzCtrl {
    int yaw;
    int pitch;
    int scan_mode;
    double step;
    bool motor_enable;    
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
    int yaw;
    int pitch;
    int motor_temperature;
    int input_voltage;
    int working_ampere;
    int longitude;
    int latitude;
}PeripheralInfo;

#endif