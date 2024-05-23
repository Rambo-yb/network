#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "network.h"
#include "network_common.h"

int SetConfig(char* sub_key, void* in, void* out, char* res, int res_size){
    return 0;
}

int GetConfig(char* sub_key, void* in, void* out, char* res, int res_size){
    if (strcmp(sub_key, "areas") == 0) {
        Areas areas;
        areas.area_num = 2;
        for (int i = 0; i < areas.area_num; i++)
        {
            snprintf(areas.area[i].area_name, sizeof(areas.area[i].area_name), "%s", i == 0 ? "abc" : "qwe");
            areas.area[i].enable = 1;
            areas.area[i].point_num_max = 8;
            areas.area[i].point_num = 4;
            for (int j = 0; j < areas.area[i].point_num; j++)
            {
                areas.area[i].point[j].x = (i+j)*100;
                areas.area[i].point[j].y = (i+j)*200;
            }
        }

        memcpy(out, &areas, sizeof(Areas));
    } else if (strcmp(sub_key, "algorithem_enable") == 0) {
        AlgorithemEnable enable;
        enable.detection_enable = 0;
        enable.tracking_enable = 1;
        enable.action_analyze_enable = 1;
        enable.tracking_object.id_num = 4;
        for (int i = 0; i < enable.tracking_object.id_num; i++)
        {
            enable.tracking_object.id[i] = i;
        }
        memcpy(out, &enable, sizeof(AlgorithemEnable));
    } else if (strcmp(sub_key, "ptz_ctrl") == 0) {
        PtzCtrl ptz_ctrl;
        ptz_ctrl.yaw = 180;
        ptz_ctrl.pitch = 0;
        ptz_ctrl.scan_mode = 1;
        ptz_ctrl.step =10.0;
        ptz_ctrl.motor_enable = 1;
        memcpy(out, &ptz_ctrl, sizeof(PtzCtrl));
    } else if (strcmp(sub_key, "chip_ctrl") == 0) {
        ChipCtrl chip_ctrl;
        chip_ctrl.contrast = 50;
        chip_ctrl.brightness = 50;
        chip_ctrl.polarity = 1;
        chip_ctrl.bad_spot_threshold = 29;
        chip_ctrl.bad_spot_oper = 2;
        chip_ctrl.hot_spot_track = 0;
        memcpy(out, &chip_ctrl, sizeof(ChipCtrl));
    } else if (strcmp(sub_key, "other_ctrl") == 0) {
        OtherCtrl other_ctrl;
        other_ctrl.defog_by_heat_enable = 1;
        memcpy(out, &other_ctrl, sizeof(OtherCtrl));
    }
    return 0;
}

int Upgrade(char* sub_key, void* in, void* out, char* res, int res_size){
    return 0;
}

int main(int argc, char** argv) {
    NetworkOperFunc func = {
        .get_config_cb = GetConfig,
        .set_config_cb = SetConfig,
        .upgrade_cb = Upgrade
    };

    NetworkInit(&func);

    while (1) {
        sleep(1);
    }

}