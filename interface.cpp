#include"interface.h"

#include "src/DingTalk/DingTalkSender.h"
#include "src/PublicPipeline/Encoding/EncodingProcessing.h"


const wchar_t *SendDingTalkMessageForMQLTest(const wchar_t *wchar_message) {
    try {
        const auto wchar_webhookURL =
                // L"http://127.0.0.1:7899/robot/send?access_token=xxxx";
                L"https://oapi.dingtalk.com/robot/send?access_token=xxxx";
        const auto wchar_secret = L"SECxxxx";
        const std::string ret = SendDingTalkMessage(
            wcharToUtf8(wchar_webhookURL),
            wcharToUtf8(wchar_secret),
            wcharToUtf8(wchar_message).c_str()
        );
        return utf8ToWchar(ret);
    } catch (const std::exception &e) {
        const std::string errorMessage = e.what();
        return utf8ToWchar(errorMessage);
    }
}

const wchar_t *SendDingTalkMessageForMQL(const wchar_t *wchar_webhookURL,
                                         const wchar_t *wchar_secret,
                                         const wchar_t *wchar_message) {
    // 确保输入参数不为 nullptr
    if (!wchar_webhookURL || !wchar_secret || !wchar_message) {
        return utf16ToWchar(u"SendDingTalkMessageForMQL传入参数不全");
    }

    try {
        const std::string ret = SendDingTalkMessage(
            wcharToUtf8(wchar_webhookURL),
            wcharToUtf8(wchar_secret),
            wcharToUtf8(wchar_message).c_str()
        );
        return utf8ToWchar(ret);
    } catch (const std::exception &e) {
        // std::cerr << "异常信息: " << e.what() << std::endl; // 输出异常信息到控制台
        const std::string errorMessage = e.what();
        return utf8ToWchar(errorMessage);
    }
}
