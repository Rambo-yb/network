#ifndef __NETWORK_H__
#define __NETWORK_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 网络操作回调
 * 回调子项与输入参数in的类型关系
 * [sub_key, in/out类型]:["string", char*],["areas", Areas],["ptz_ctrl", PtzCtrl],["chip_ctrl", ChipCtrl],
 *                   ["other_ctrl", OtherCtrl],["algorithem_enable", AlgorithemEnable]
 * @param [in] sub_key 回调子项类型,                   
 * @param [in] in 对应子项输入
 * @param [out] out 对应子项输出, 不接收输出时，传入NULL
 * @param [out] res 输出结果buff
 * @param [in] res_size 输出结果buff大小
 * @return 成功返回 0
 *         失败返回 其他值 
*/
typedef int (*NetworkOperCb)(char* sub_key, void* in, void* out, char* res, int res_size);

typedef struct NetworkOperFunc
{
    NetworkOperCb set_config_cb;
    NetworkOperCb get_config_cb;
    NetworkOperCb upgrade_cb;
}NetworkOperFunc;

/**
 * @brief 网络初始化
 * @return 成功返回 0
 *         失败返回 其他值
*/
int NetworkInit(NetworkOperFunc* func);

/**
 * @brief 网络反初始化
*/
void NetworkUnInit();

/**
 * @brief 网络请求函数
 * url==NULL，使用pc下发的地址，必须携带req_key和boby结构体。[req_key, boby类型]:["alarm_info", AlarmInfo],["peripheral_info", PeripheralInfo]
 * url!=NULL，想指定地址发送请求，传入的boby只能为char*字符串，无boby为GET请求
 * @param [in] url 请求地址
 * @param [in] req_key 请求关键字, 传入指定url时，req_key=unused
 * @param [in] boby 对应请求体, boby为NULL时，使用GET请求
 * @param [out] res 返回执行结果或回复数据
 * @param [in] res_size 返回结果buff大小
 * @param [in] timeout 超时时间，ms
 * @return 成功返回 0
 *         失败返回 其他值
*/
int NetworkRequest(const char* url, const char* req_key, void* body, char* res, int res_size, int timeout);

#ifdef __cplusplus
};
#endif

#endif