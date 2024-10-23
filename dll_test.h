//
// Created by Ranranran on 24-10-22.
//

#ifndef TEST_MAIN_H
#define TEST_MAIN_H
#include <filesystem>
#include <iostream>
#include <windows.h>

#include "src/PublicPipeline/Encoding/EncodingProcessing.h"
// #include "src/PublicPipeline/Encoding/EncodingProcessing.cpp"

// 获取当前时间并格式化为字符串（返回 std::string）
inline std::string getCurrentTime();

// 获取当前路径
inline std::string getCurrentPath();

#endif //TEST_MAIN_H
