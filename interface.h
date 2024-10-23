//
// Created by Ranranran on 24-10-6.
//

// #include "interface.h"

#ifndef INTERFACE_H
#define INTERFACE_H

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __attribute__((visibility("default")))
#endif

#include <string>

extern "C" {
/**
 * 通过webhook给钉钉发送消息
 * @param wchar_webhookURL 钉钉webhook网址
 * @param wchar_secret SER
 * @param wchar_message 消息
 * @return 钉钉webhook返回值
 */
EXPORT const wchar_t *SendDingTalkMessageForMQL(const wchar_t *wchar_webhookURL,
                                                const wchar_t *wchar_secret,
                                                const wchar_t *wchar_message);

/**
 * 调试用，函数内部定义api接口
 * @param wchar_message 消息体
 * @return 钉钉返回结果
 */
EXPORT const wchar_t *SendDingTalkMessageForMQLTest(const wchar_t *wchar_message);
}

#endif // INTERFACE_H
