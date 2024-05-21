#ifndef __HTTP_CLIENT_H__
#define __HTTP_CLIENT_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief http请求
 * @param [in] url http请求地址
 * @param [in] body http请求体，如body=NULL，请求为GET请求
 * @param [out] res http请求结果
 * @param [in] res_size http请求结果buff大小
 * @param [in] timeout 超时时间
 * @return 返回http请求码
*/
int HttpClientRequest(char* url, char* body, char* res, int res_size, int timeout);

#ifdef __cplusplus
};
#endif

#endif