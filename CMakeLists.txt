cmake_minimum_required(VERSION 3.29)
project(MetaTraderMessageShare)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /DNDEBUG") # 优化构建大小 在 Release 模式下禁用断言
set(CMAKE_CXX_STANDARD_REQUIRED ON) #  强制要求C++标准
set(CMAKE_CXX_EXTENSIONS OFF) # 生成的代码仅使用标准 C++ 语法
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FILE_OFFSET_BITS=64") # 64位偏移量
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_LARGEFILE_SOURCE=1") # 大文件支持

set(CONAN_DISABLE_CHECK_COMPILER ON)
#set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)# 全部函数导出
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")# 启用所有的编译器警告

find_package(ICU REQUIRED)
find_package(CURL REQUIRED)
find_package(cJSON REQUIRED)
find_package(OpenSSL REQUIRED)

add_library(${PROJECT_NAME} SHARED
        interface.cpp
        interface.h
        src/DingTalk/DingTalkSender.cpp
        src/DingTalk/DingTalkSender.h
        src/PublicPipeline/Encoding/EncodingProcessing.cpp
        src/PublicPipeline/Encoding/EncodingProcessing.h
)

# 导出
target_compile_definitions(${PROJECT_NAME} PRIVATE MT5DLL_EXPORTS)


target_link_libraries(${PROJECT_NAME}
        CURL::libcurl
        OpenSSL::SSL
        OpenSSL::Crypto
        cjson::cjson
        icu::icu
)

# 添加调试测试程序
if (CMAKE_BUILD_TYPE STREQUAL "Debug") # debug条件下执行
    add_executable(DLL_Test
            dll_test.cpp
            dll_test.h
            src/DingTalk/DingTalkSender.cpp
            src/DingTalk/DingTalkSender.h
            src/PublicPipeline/Encoding/EncodingProcessing.cpp
            src/PublicPipeline/Encoding/EncodingProcessing.h
    )
    target_link_libraries(DLL_Test ${PROJECT_NAME})

    add_executable(Main_Test
            main_test.cpp
            src/DingTalk/DingTalkSender.cpp
            src/DingTalk/DingTalkSender.h
            src/PublicPipeline/Encoding/EncodingProcessing.cpp
            src/PublicPipeline/Encoding/EncodingProcessing.h
    )
    target_link_libraries(Main_Test ${PROJECT_NAME})

endif ()