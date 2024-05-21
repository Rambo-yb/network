#ifndef __HTTP_SERVER_H__
#define __HTTP_SERVER_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief http url处理回调
 * @param [in] in http协议接收的数据
 * @param [out] out http协议回复数据
 * @param [in] out_size http协议回复buff大小
 * @param [out] res http执行结果消息
 * @param [in] res_size http执行结果消息buff大小
 * @return 成功返回 200
 *         失败返回 其他值
 */
typedef int (*HttpServerUrlProcCb) (char* in, char* out, int out_size, char* res, int res_size);

/**
 * @brief http server初始化函数
 * @param [in] addr http服务器监听地址，http://addr:port
 * @return 成功返回0
 *         失败返回其他值
*/
int HttpServerInit(char *addr);

/**
 * @brief 释放http server函数
*/
void HttpServerUnInit();

/**
 * @brief http url注册函数
 * @param [in] method http请求类型，"POST" / "GET" ...
 * @param [in] url http请求地址
 * @param [in] cb http请求处理回调
 * @return
 */
void HttpServerUrlRegister(char* method, char* url, HttpServerUrlProcCb cb);

#ifdef __cplusplus
};
#endif

#endif