#ifndef __HTTP_CLIENT_H__
#define __HTTP_CLIENT_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief http post/get请求
 * @param [in] method 请求类型
 * @param [in] url http请求地址
 * @param [in] body http请求体
 * @param [out] res http请求结果
 * @param [in] res_size http请求结果buff大小
 * @param [in] timeout 超时时间
 * @return 成功返回0
*/
int HttpRequest(const char* method, const char* url, const char* body, char* res, int res_size, int timeout);



/**
 * @brief form-data请求
 * @param [in] url http请求地址
 * @param [in] timeout 超时时间
 * @return 返回操作句柄，失败返回NULL
*/
void* HttpFormDataRequestInit(const char* url, int timeout);

/**
 * @brief 添加form-data请求消息头
 * @param [in] handle 操作句柄
 * @param [in] header 消息头
*/
void HttpFormDataRequestHeader(void* handle, const char* header);

/**
 * @brief 添加form-data请求消息体
 * @param [in] handle 操作句柄
 * @param [in] type 消息体类型, text/file
 * @param [in] key 消息体关键字
 * @param [in] val 数据
 * @param [in] len 数据长度
*/
void HttpFormDataRequestBody(void* handle, char* type, const char* key, const char* val, int len);

/**
 * @brief 执行form-data请求
 * @param [in] handle 操作句柄
 * @param [out] res http请求结果
 * @param [in] res_size http请求结果buff大小
 * @return 成功返回0
*/
int HttpFormDataExec(void* handle, char* res, int res_size);

/**
 * @brief 释放form-data请求
 * @param [in] handle 操作句柄
*/
void HttpFormDataRequestUnInit(void* handle);

#ifdef __cplusplus
};
#endif

#endif