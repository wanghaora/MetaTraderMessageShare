# 说明

本程序构建生成为MetatTrader 4/5提供发送消息的动态库

## 方法一：clion构建

如果MinGW无法构建，安装Visual Studio，用clion调用Visual Studio的工具链。

cmake选项：

```C
-G Ninja -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES="conan_provider.cmake"
```

![image-20241023155705695](readme.picture\image-20241023155705695.png)



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



## 使用：

MQL4/5代码：

```c
#property description "--------------------------------"
#property copyright "揣手手"
#property version "1.0"
#ifndef P
#define P(X) Print(#X, " = ", X)
#endif

// 换成你自己的路径，如在MQL5(MQL4)的Libraries路径下可以使用相对路径
// 此处我MQL5(MQL4)文件夹下有message，message里含有MetaTraderMessageShare.dll
// 此处也可用相对路径 如 "D:/xxxx/MetaTraderMessageShare.dll"
#import "message/MetaTraderMessageShare.dll" //换成你自己的路径，或者在MQL的include路径下可用相对路径 如 "D:/ttttt/MetaTraderMessageShare.dll"
string SendDingTalkMessageForMQL(string url, string sec, string msg);
#import
string URL = "https://oapi.dingtalk.com/robot/send?access_token=xxxx";
string SECRET = "SECxxxxxx";
string msg = "123abc汉字";

void OnStart() {
    string ret = SendDingTalkMessageForMQL(URL, SECRET, msg);
    P(ret);
}

```

## 注意

本库禁用了HTTPS 的校验，若想启用需要更改如下代码

MetaTraderMessageShare/src/DingTalk/DingTalkSender.cpp

```C
// 不验证对等证书        
curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); 
// 不验证主机
curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); 
        //更改为
// 验证对等证书
curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 2L); 
// 验证主机
curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L); 
```

同时需要指定证书路径有效

下面是从 Mozilla 提取的 CA 证书https://curl.se/docs/caextract.html

默认文件cacert.pem和MetaTraderMessageShare.dll在同级目录内
