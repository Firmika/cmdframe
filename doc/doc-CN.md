# CmdFrame说明文档

## 1 什么是CmdFrame?

TODO...

## 2 快速入门

TODO...

```c++
#include "cmdframe_manager.h"
#include <iostream>
using namespace std;

void Add();
void Sub();
int main() {
    // Step 1: 新建一个CmdFrame管理器并导入CmdFrame
    // 在构造函数中直接导入3个CmdMenu(CmdFrame的子类)
    // FrameID: 0-2
    CmdFrameManager manager(
        {
            new CmdMenu({"To Menu 1", "To Menu 2", "Exit"}, "Main Menu",
                        "Description: Main Menu", true),
            new CmdMenu({"Hello world!", "World hello!", "Cancel"}, "Menu 1",
                        "Description Menu 1"),
            new CmdMenu({"Cal a+b", "Cal a-b", "Cancel"}, "Menu 2",
                        "Description Menu 2"),
        },
        true);  // 容纳的CmdFrame内存由管理器管理
    // 导入一个新的CmdFrame(绑定到一个lambda函数), key为hello_world
    // FrameID: 3
    manager.Append("hello_world", new CmdFrame(FINF_BACK, []() {
                       cout << "Hello World!" << endl;
                       getchar();
                   }));
    // 导入一个新的CmdFrame(绑定到一个lambda函数), key为world_hello
    // FrameID: 4
    manager.Append("world_hello", new CmdFrame([]() {
                       cout << "World Hello!" << endl;
                       getchar();
                       return FINF_BACK;
                   }));
    // 导入一个新的CmdFrame(绑定到函数Add), key为add
    // FrameID: 5
    manager.Append("add", new CmdFrame(FINF_BACK, &Add));
    // 导入一个新的CmdFrame(绑定到函数Sub), key为sub
    // FrameID: 6
    manager.Append("sub", new CmdFrame(&Sub));
    // Step 2: 指定CmdFrame之间的链接关系
    // 每个tuple三个参数分别指定: 源Frame, 源Frame接口, 目标Frame
    // 此处为{源Frame ID, 源Frame接口ID, 目标Frame ID}
    manager.Link(std::vector<std::tuple<int, int, int>>{
        {0, FINF_FRAME(0), 1},
        {0, FINF_FRAME(1), 2},
        {0, FINF_FRAME(2), FID_EXIT},  // 特殊Frame: 退出manager
        {1, FINF_FRAME(2), FID_BACK},  // 特殊Frame: 回到上一个Frame
        {2, FINF_FRAME(2), FID_BACK},
    });
    // 每个tuple三个参数分别指定: 源Frame, 源Frame接口, 目标Frame
    // 此处为{源Frame ID, 源Frame接口ID, 目标Frame key}
    manager.Link(std::vector<std::tuple<int, int, std::string>>{
        {1, FINF_FRAME(0), "hello_world"},
        {1, FINF_FRAME(1), "world_hello"},
        {2, FINF_FRAME(0), "add"},
        {2, FINF_FRAME(1), "sub"},
    });
    // Step 3: 启动管理器
    manager.Execute();
    return 0;
}

void Add() {
    int a, b;
    cout << "Input a, b: ";
    cin >> a >> b;
    cout << "a + b = " << a + b << endl;
    getchar();
    getchar();
}

int Sub() {
    int a, b;
    cout << "Input a, b: ";
    cin >> a >> b;
    cout << "a - b = " << a - b << endl;
    getchar();
    getchar();
    return FINF_BACK;
}
```


## 3 `CmdFrame`

TODO...

## 4 `CmdMenu`

TODO...

## 5 `CmdFrameManager`

TODO...

