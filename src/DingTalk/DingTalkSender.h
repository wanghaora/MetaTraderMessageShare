//
// Created by Ranranran on 24-10-6.
//

#ifndef DINGTALKSENDER_H
#define DINGTALKSENDER_H

#include <chrono> // 时间 API
#include <iostream>

#include "cjson/cjson.h"
#include "curl/curl.h"
#include "openssl/buffer.h" // Base64 编码
#include "openssl/hmac.h"  // HMAC SHA256 签名

std::string SendDingTalkMessage(const std::string &, const std::string &, const char *); // 发送消息

size_t WriteCallback(void *, size_t, size_t, void *); // 写入回调函数

std::string generateSign(const std::string &, const std::string &); // 生成 HMAC SHA256 签名

std::string base64_encode(const std::string &); // Base64 编码

#endif //DINGTALKSENDER_H
