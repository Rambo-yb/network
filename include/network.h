#ifndef __NETWORK_H__
#define __NETWORK_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 网络操作回调
 * @param [in] sub_key 回调子项类型
 * @param [in] in 对应子项输入
 * @param [out] out 对应子项输出
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
    NetworkOperCb dev_ctrl_cb;
    NetworkOperCb algorithm_enable_cb;
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
 * @param [in] url 请求地址，url=NULL时使用pc下发的地址
 * @param [in] req_key 请求关键字
 * @param [in] boby 对应请求体
 * @param [out] res 返回执行结果或回复数据
 * @param [in] res_size 返回结果buff大小
 * @return 成功返回 0
 *         失败返回 其他值
*/
int NetworkRequest(const char* url, char* req_key, void* body, char* res, int res_size);

#ifdef __cplusplus
};
#endif

#endif