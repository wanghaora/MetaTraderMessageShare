//
// Created by Ranranran on 24-10-6.
//
#include "DingTalkSender.h"

// Base64 编码
std::string base64_encode(const std::string &in) {
    BUF_MEM *bufferPtr;

    BIO *b64 = BIO_new(BIO_f_base64());
    BIO *bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); // 不要换行

    BIO_write(bio, in.data(), static_cast<int>(in.size()));
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);
    BIO_set_close(bio, BIO_NOCLOSE);
    BIO_free_all(bio);

    return {bufferPtr->data, bufferPtr->length};
}

// 生成 HMAC SHA256 签名
std::string generateSign(const std::string &timestamp, const std::string &secret) {
    const std::string stringToSign = timestamp + "\n" + secret;

    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digestLength;

    HMAC(EVP_sha256(), secret.c_str(), static_cast<int>(secret.length()),
         reinterpret_cast<const unsigned char *>(const_cast<char *>(stringToSign.c_str())), stringToSign.length(),
         digest, &digestLength);

    return base64_encode(std::string(reinterpret_cast<char *>(digest), digestLength));
}

// 写入回调函数
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    static_cast<std::string *>(userp)->append(static_cast<char *>(contents), size * nmemb);
    return size * nmemb;
}

// 发送 DingTalk 消息
std::string SendDingTalkMessage(const std::string &webhookURL, const std::string &secret, const char *message) {
    std::string readBuffer;

    // 获取当前时间戳
    const auto now = std::chrono::system_clock::now();
    const auto timestamp = std::to_string(
        std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count());

    // 生成签名
    std::string sign = generateSign(timestamp, secret);

    // 构造完整 URL
    const std::string fullURL = webhookURL + "&timestamp=" + timestamp + "&sign=" + sign;

    // 创建 JSON 消息体
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "msgtype", "text");
    cJSON *text = cJSON_CreateObject();
    cJSON_AddStringToObject(text, "content", message);
    cJSON_AddItemToObject(root, "text", text);
    char *jsonString = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    // 初始化libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);

    std::string responseData;

    if (CURL *curl = curl_easy_init()) {
        // 设置请求URL
        curl_easy_setopt(curl, CURLOPT_URL, fullURL.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER,
                         curl_slist_append(nullptr, "Content-Type: application/json;charset=utf-8"));
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // 禁用验证对等证书 2L为启用
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // 禁用验证主机 2L为启用
        curl_easy_setopt(curl, CURLOPT_CAINFO, "./cacert.pem");

        // 执行请求
        const CURLcode res = curl_easy_perform(curl);

        // 检查请求结果
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        // 清理
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    free(jsonString); // 释放内存

    return responseData; // 返回响应数据
}
