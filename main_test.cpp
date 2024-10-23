#include <iostream>

#include "src/DingTalk/DingTalkSender.h"
#include "src/PublicPipeline/Encoding/EncodingProcessing.h"


int main() {
    const std::string webhookURL =
            // "https://oapi.dingtalk.com/robot/send?access_token=xxxx";
            "https://oapi.dingtalk.com/robot/send?access_token=xxxxx";
    const std::string secret = "SECxxxxx";
    const wchar_t *msg = L"abc你好123";

    const std::string ret = SendDingTalkMessage(webhookURL, secret, wcharToUtf8(msg).c_str());
    std::cout << "响应结果: " << ret << std::endl;
    return 0;
}
