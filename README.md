# 说明

本程序构建生成为MetatTrader 4/5提供发送消息的动态库

## 方法一：clion构建

安装	

```C
-DCMAKE_PROJECT_TOP_LEVEL_INCLUDES="conan_provider.cmake"
```

```C
-G Ninja -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES="conan_provider.cmake"
```





## 方法二 ：手动构建


```bash

if exist build rmdir /s /q build
mkdir build
cd build
cmake .. -G Ninja -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES="conan_provider.cmake"

```
解释：
if exist build：检查 build 目录是否存在。
rmdir /s /q build：如果存在，删除 build 目录及其所有内容。
/s：删除指定目录及所有子目录和文件。
/q：静默模式，不提示确认。
mkdir build：重新创建 build 目录。

使用 Ninja 构建项目

