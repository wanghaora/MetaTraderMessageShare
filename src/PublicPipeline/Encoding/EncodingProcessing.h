//
// Created by Ranranran on 24-10-21.
//

#ifndef ENCODINGPROCESSING_H
#define ENCODINGPROCESSING_H
#include <codecvt>
#include <iostream>
#include <locale>
#include <vector>
#include <unicode/ucnv.h>  // ICU Converter
#include <unicode/unistr.h> // ICU UnicodeString

// 将UTF-16字符串转换为UTF-8字符串（不添加BOM）
std::string utf16ToUtf8(const std::u16string &utf16Str);

// 将UTF-8字符串转换为UTF-16字符串（不添加BOM）
std::u16string utf8ToUtf16(const std::string &utf8Str);

// 将UTF-8字符串转换为GBK字符串
std::string utf8ToGbk(const std::string &utf8Str);

// 将GBK字符串转换为UTF-8字符串
std::string gbkToUtf8(const std::string &gbkStr);

// 添加 UTF-8 BOM
std::string addUtf8BOM(const std::string &utf8Str);

// 去除 UTF-8 BOM
std::string removeUtf8BOM(const std::string &utf8Str);

// 添加 UTF-16 BOM (小端)
std::u16string addUtf16BOM(const std::u16string &utf16Str);

// 去除 UTF-16 BOM
std::u16string removeUtf16BOM(const std::u16string &utf16Str);

// UTF-16 转 wchar_t
const wchar_t *utf16ToWchar(const std::u16string &utf16Str);

// wchar_t 转 UTF-16
std::u16string wcharToUtf16(const wchar_t *wstr);

// UTF-8 转 wchar_t
const wchar_t *utf8ToWchar(const std::string &utf8Str);

// wchar_t 转 UTF-8
std::string wcharToUtf8(const wchar_t *wstr);

#endif //ENCODINGPROCESSING_H
