#include"dll_test.h"



int main() {
    // 加载 DLL
    HMODULE hModule = LoadLibrary(TEXT((getCurrentPath() + "/MetaTraderMessageShare.dll").c_str()));
    if (!hModule) {
        std::cerr << "无法加载 DLL: " << GetLastError() << std::endl;
        return 1;
    }

    // 获取函数地址
    typedef const wchar_t * (*SendDingTalkMessageForMQLFunc)(const wchar_t *, const wchar_t *, const wchar_t *);
    auto SendDingTalkMessageForMQL =
            reinterpret_cast<SendDingTalkMessageForMQLFunc>(GetProcAddress(hModule, "SendDingTalkMessageForMQL"));
    if (!SendDingTalkMessageForMQL) {
        std::cerr << "无法获取 SendDingTalkMessageForMQL 函数地址: " << GetLastError() << std::endl;
        FreeLibrary(hModule);
        return 1;
    }

    // 准备输入数据
    const wchar_t *webhookURL = L"http://127.0.0.1:7899/robot/send?access_token=xxx";
    const wchar_t *secret = L"xxx";
    // 拼接 const wchar_t* 和 std::wstring
    std::wstring fullMessage = std::wstring(L"测试消息") + utf8ToWchar(getCurrentTime());
    const wchar_t* message = fullMessage.c_str();


    // 调用函数并输出返回值
    if (const wchar_t *result = SendDingTalkMessageForMQL(webhookURL, secret, message)) {
        std::cout << "SendDingTalkMessageForMQL 返回结果: " << wcharToUtf8(result) << std::endl;
    } else {
        std::wcerr << L"SendDingTalkMessageForMQL 返回空结果" << std::endl;
    }

    // 卸载 DLL
    FreeLibrary(hModule);

    return 0;
}

// 获取当前路径
inline std::string getCurrentPath() {
    std::filesystem::path currentPath = std::filesystem::current_path();
    return currentPath.string();
}

// 获取当前时间并格式化为字符串（返回 std::string）
inline std::string getCurrentTime() {
    std::stringstream ss;
    std::time_t now = std::time(nullptr);
    std::tm localTime{};
    localtime_s(&localTime, &now); // 获取本地时间

    ss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S"); // 时间格式: 年-月-日 时:分:秒
    return ss.str();
}
