#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdbool.h>

#define AREA_NAME_SIAE (128)
#define POINT_NUM_MAX (8)
#define AREA_NUM_MAX (32)
#define TRACK_ID_NUM_MAX (128)

typedef struct Point {
    int x;
    int y;
}Point;

typedef struct Area {
    char area_name[AREA_NAME_SIAE];
    bool enable;
    int point_num_max;
    int point_num;
    Point point[POINT_NUM_MAX];
}Area;

typedef struct Areas {
    int area_num;
    Area area[AREA_NUM_MAX];
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
    bool bad_spot_remove;
    bool hot_spot_track;
}ChipCtrl;

typedef struct OtherCtrl {
    bool defog_by_heat_enable;
}OtherCtrl;

typedef struct TrackingObject {
    int id_num;
    int id[TRACK_ID_NUM_MAX];
}TrackingObject;

typedef struct AlgorithemEnable {
    bool detection_enable;
    bool tracking_enable;
    bool action_analyze_enable;
    TrackingObject tracking_object;
}AlgorithemEnable;

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