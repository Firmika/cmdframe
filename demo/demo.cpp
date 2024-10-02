#include "cmdframe_manager.h"
#include <iostream>

using namespace std;
void TestMenu() {
    CmdMenu menu({"Option 0", "Option 1", "Option 2"}, "Title", "Description");
    int ret = menu.Execute();
    cout << "Selected: " << ret << endl;
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

int main() {
    CmdFrameManager manager(
        {
            new CmdMenu({"To Menu 1", "To Menu 2", "Exit"}, "Main Menu",
                        "Description: Main Menu", true),
            new CmdMenu({"Hello world!", "World hello!", "Cancel"}, "Menu 1",
                        "Description Menu 1"),
            new CmdMenu({"Cal a+b", "Cal a-b", "Cancel"}, "Menu 2",
                        "Description Menu 2"),
        },
        true);
    manager.Append("hello_world", new CmdFrame(FINF_BACK, []() {
                       cout << "Hello World!" << endl;
                       getchar();
                   }));
    manager.Append("world_hello", new CmdFrame([]() {
                       cout << "World Hello!" << endl;
                       getchar();
                       return FINF_BACK;
                   }));
    manager.Append("add", new CmdFrame(FINF_BACK, &Add));
    manager.Append("sub", new CmdFrame(&Sub));
    manager.Link(std::vector<std::tuple<int, int, int>>{
        {0, FINF_FRAME(0), 1},
        {0, FINF_FRAME(1), 2},
        {0, FINF_FRAME(2), FID_EXIT},
        {1, FINF_FRAME(2), FID_BACK},
        {2, FINF_FRAME(2), FID_BACK},
    });
    manager.Link(std::vector<std::tuple<int, int, std::string>>{
        {1, FINF_FRAME(0), "hello_world"},
        {1, FINF_FRAME(1), "world_hello"},
        {2, FINF_FRAME(0), "add"},
        {2, FINF_FRAME(1), "sub"},
    });
    manager.Execute();
    return 0;
}