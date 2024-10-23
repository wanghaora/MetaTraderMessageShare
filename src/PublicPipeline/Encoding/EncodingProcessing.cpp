//
// Created by Ranranran on 24-10-21.
//

#include "EncodingProcessing.h"


// 将UTF-16字符串转换为UTF-8字符串（不添加BOM）
std::string utf16ToUtf8(const std::u16string &utf16Str) {
    UErrorCode errorCode = U_ZERO_ERROR;

    // 创建转换器：从UTF-16（小端）到UTF-8
    UConverter *utf16Converter = ucnv_open("UTF-16LE", &errorCode); // 使用小端
    UConverter *utf8Converter = ucnv_open("UTF-8", &errorCode);
    if (U_FAILURE(errorCode)) {
        std::cerr << "转换器创建失败！" << std::endl;
        return {};
    }

    // 转换UTF-16到UTF-8
    const int32_t utf8Len = ucnv_fromUChars(utf8Converter, nullptr, 0,
                                            utf16Str.c_str(),
                                            static_cast<long>(utf16Str.length()), &errorCode);
    errorCode = U_ZERO_ERROR; // 重置错误状态
    std::vector<char> utf8Buffer(utf8Len + 1); // 为UTF-8字符串分配足够的空间
    ucnv_fromUChars(utf8Converter, utf8Buffer.data(), utf8Len + 1, utf16Str.c_str(),
                    static_cast<long>(utf16Str.length()), &errorCode);

    if (U_FAILURE(errorCode)) {
        std::cerr << "转换失败！" << std::endl;
        ucnv_close(utf16Converter);
        ucnv_close(utf8Converter);
        return {};
    }

    // 关闭转换器
    ucnv_close(utf16Converter);
    ucnv_close(utf8Converter);

    // 使用大括号返回，避免重复返回类型
    return {utf8Buffer.data(), static_cast<size_t>(utf8Len)};
}

// 将UTF-8字符串转换为UTF-16字符串（不添加BOM）
std::u16string utf8ToUtf16(const std::string &utf8Str) {
    UErrorCode errorCode = U_ZERO_ERROR;

    // 创建UTF-8转换器
    UConverter *utf8Converter = ucnv_open("UTF-8", &errorCode);
    if (U_FAILURE(errorCode)) {
        std::cerr << "UTF-8转换器创建失败！" << std::endl;
        return u"";
    }

    // 计算UTF-16所需的缓冲区大小
    const int32_t utf16Len = ucnv_toUChars(utf8Converter, nullptr, 0, utf8Str.c_str(),
                                           static_cast<long>(utf8Str.length()), &errorCode);
    if (errorCode != U_BUFFER_OVERFLOW_ERROR && U_FAILURE(errorCode)) {
        std::cerr << "计算UTF-16长度失败！" << std::endl;
        ucnv_close(utf8Converter);
        return u"";
    }
    errorCode = U_ZERO_ERROR; // 重置错误状态

    // 分配UTF-16缓冲区
    std::vector<UChar> utf16Buffer(utf16Len);

    // 将UTF-8转换为UTF-16
    ucnv_toUChars(utf8Converter, utf16Buffer.data(), utf16Len, utf8Str.c_str(), static_cast<long>(utf8Str.length()),
                  &errorCode);
    if (U_FAILURE(errorCode)) {
        std::cerr << "UTF-8到UTF-16转换失败！" << std::endl;
        ucnv_close(utf8Converter);
        return u"";
    }

    // 关闭转换器
    ucnv_close(utf8Converter);

    // 返回UTF-16字符串
    return {utf16Buffer.data(), static_cast<size_t>(utf16Len)};
}

// 将UTF-8字符串转换为GBK字符串
std::string utf8ToGbk(const std::string &utf8Str) {
    UErrorCode errorCode = U_ZERO_ERROR;

    // 创建UTF-8到GBK的转换器
    UConverter *utf8Converter = ucnv_open("UTF-8", &errorCode);
    UConverter *gbkConverter = ucnv_open("GB18030", &errorCode); // ICU中使用GB18030来表示GBK
    if (U_FAILURE(errorCode)) {
        std::cerr << "转换器创建失败！" << std::endl;
        return {};
    }

    // 将UTF-8字符串转换为UTF-16
    // 修正：显式地将大小转换为int32_t，并检查是否超过范围
    if (utf8Str.size() > static_cast<size_t>(std::numeric_limits<int32_t>::max())) {
        std::cerr << "UTF-8字符串太长，无法处理！" << std::endl;
        return {};
    }

    std::vector<UChar> utf16Buffer(static_cast<int32_t>(utf8Str.size()));
    const int32_t utf16Len = ucnv_toUChars(utf8Converter, utf16Buffer.data(), static_cast<long>(utf16Buffer.size()),
                                           utf8Str.c_str(), static_cast<int32_t>(utf8Str.size()), &errorCode);
    if (U_FAILURE(errorCode)) {
        std::cerr << "UTF-8到UTF-16转换失败！" << std::endl;
        ucnv_close(utf8Converter);
        ucnv_close(gbkConverter);
        return {};
    }

    // 计算GBK所需的缓冲区大小
    const int32_t gbkLen = ucnv_fromUChars(gbkConverter, nullptr, 0, utf16Buffer.data(), utf16Len, &errorCode);
    errorCode = U_ZERO_ERROR; // 重置错误状态
    std::vector<char> gbkBuffer(static_cast<size_t>(gbkLen) + 1); // +1是为了保存null终止符
    ucnv_fromUChars(gbkConverter, gbkBuffer.data(), gbkLen + 1, utf16Buffer.data(), utf16Len, &errorCode);

    if (U_FAILURE(errorCode)) {
        std::cerr << "UTF-16到GBK转换失败！" << std::endl;
        ucnv_close(utf8Converter);
        ucnv_close(gbkConverter);
        return {};
    }

    // 关闭转换器
    ucnv_close(utf8Converter);
    ucnv_close(gbkConverter);

    return {gbkBuffer.data(), static_cast<size_t>(gbkLen)};
}

// 将GBK字符串转换为UTF-8字符串
std::string gbkToUtf8(const std::string &gbkStr) {
    UErrorCode errorCode = U_ZERO_ERROR;

    // 创建GBK到UTF-8的转换器
    UConverter *gbkConverter = ucnv_open("GB18030", &errorCode); // ICU中使用GB18030来表示GBK
    UConverter *utf8Converter = ucnv_open("UTF-8", &errorCode);
    if (U_FAILURE(errorCode)) {
        std::cerr << "转换器创建失败！" << std::endl;
        return {};
    }

    // 将GBK字符串转换为UTF-16
    // 修正：显式地将大小转换为int32_t，并检查是否超过范围
    if (gbkStr.size() > static_cast<size_t>(std::numeric_limits<int32_t>::max())) {
        std::cerr << "GBK字符串太长，无法处理！" << std::endl;
        return {};
    }

    std::vector<UChar> utf16Buffer(static_cast<int32_t>(gbkStr.size()));
    const int32_t utf16Len = ucnv_toUChars(gbkConverter, utf16Buffer.data(), static_cast<long>(utf16Buffer.size()),
                                           gbkStr.c_str(), static_cast<int32_t>(gbkStr.size()), &errorCode);
    if (U_FAILURE(errorCode)) {
        std::cerr << "GBK到UTF-16转换失败！" << std::endl;
        ucnv_close(gbkConverter);
        ucnv_close(utf8Converter);
        return {};
    }

    // 计算UTF-8所需的缓冲区大小
    const int32_t utf8Len = ucnv_fromUChars(utf8Converter, nullptr, 0, utf16Buffer.data(), utf16Len, &errorCode);
    errorCode = U_ZERO_ERROR; // 重置错误状态
    std::vector<char> utf8Buffer(static_cast<size_t>(utf8Len) + 1); // +1 是为了保存null终止符
    ucnv_fromUChars(utf8Converter, utf8Buffer.data(), utf8Len + 1, utf16Buffer.data(), utf16Len, &errorCode);

    if (U_FAILURE(errorCode)) {
        std::cerr << "UTF-16到UTF-8转换失败！" << std::endl;
        ucnv_close(gbkConverter);
        ucnv_close(utf8Converter);
        return {};
    }

    // 关闭转换器
    ucnv_close(gbkConverter);
    ucnv_close(utf8Converter);

    return {utf8Buffer.data(), static_cast<size_t>(utf8Len)};
}

// 添加 UTF-8 BOM
std::string addUtf8BOM(const std::string &utf8Str) {
    const std::string utf8BOM = "\xEF\xBB\xBF"; // UTF-8 BOM
    return utf8BOM + utf8Str;
}

// 去除 UTF-8 BOM
std::string removeUtf8BOM(const std::string &utf8Str) {
    if (const std::string utf8BOM = "\xEF\xBB\xBF"; utf8Str.compare(0, utf8BOM.size(), utf8BOM) == 0) {
        return utf8Str.substr(utf8BOM.size());
    }
    return utf8Str; // 如果没有BOM，返回原字符串
}

// 添加 UTF-16 BOM (小端)
std::u16string addUtf16BOM(const std::u16string &utf16Str) {
    constexpr char16_t utf16BOM = 0xFEFF; // UTF-16 小端 BOM
    return std::u16string(1, utf16BOM) + utf16Str;
}

// 去除 UTF-16 BOM
std::u16string removeUtf16BOM(const std::u16string &utf16Str) {
    if (!utf16Str.empty() && utf16Str[0] == 0xFEFF) {
        return utf16Str.substr(1);
    }
    return utf16Str; // 如果没有BOM，返回原字符串
}

// UTF-16 转 wchar_t
const wchar_t *utf16ToWchar(const std::u16string &utf16Str) {
    // 分配新的 wchar_t 数组（需要+1用于空终止符）
    auto *wstr = new wchar_t[utf16Str.size() + 1];

    // 将 std::u16string 转换为 wchar_t
    for (size_t i = 0; i < utf16Str.size(); ++i) {
        wstr[i] = static_cast<wchar_t>(utf16Str[i]);
    }

    // 添加空终止符
    wstr[utf16Str.size()] = L'\0';

    return wstr;
}

// wchar_t 转 UTF-16
std::u16string wcharToUtf16(const wchar_t *wstr) {
    std::u16string utf16Str;

    // 将 wchar_t 转换为 std::u16string
    while (*wstr) {
        utf16Str += static_cast<char16_t>(*wstr);
        ++wstr;
    }

    return utf16Str;
}

// UTF-8 转 wchar_t
const wchar_t *utf8ToWchar(const std::string &utf8Str) {
    // 使用 ICU 将 UTF-8 转换为 UTF-16（wchar_t）
    icu::UnicodeString unicodeStr = icu::UnicodeString::fromUTF8(utf8Str);
    // 分配新的 wchar_t 数组
    auto *wstr = new wchar_t[unicodeStr.length() + 1];

    // 将 UnicodeString 转换为 wchar_t
    for (int32_t i = 0; i < unicodeStr.length(); ++i) {
        wstr[i] = static_cast<wchar_t>(unicodeStr[i]);
    }

    // 添加空终止符
    wstr[unicodeStr.length()] = L'\0';

    return wstr;
}

// wchar_t 转 UTF-8
std::string wcharToUtf8(const wchar_t *wstr) {
    // 使用 ICU 将 wchar_t 转换为 UTF-8
    icu::UnicodeString unicodeStr = icu::UnicodeString(wstr);
    std::string utf8Str;
    unicodeStr.toUTF8String(utf8Str);
    return utf8Str;
}
