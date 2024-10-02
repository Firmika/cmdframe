#include "frameutils.h"
#include <functional>
#include <iostream>
#include <algorithm>
#include <sstream>

using std::cin;
using std::cout;
using std::endl;
using std::string;

void frameutils::Pause(const string &prompt) {
    cout << prompt << std::endl;
    frameutils::Getch();
}

string frameutils::Trim(string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    auto it = std::remove_if(str.begin(), str.end(),
                             [](char ch) { return !(ch >= 'a' && ch <= 'z'); });
    str.erase(it, str.end());
    return str;
}

#if defined(_WIN32) || defined(_WIN64)
#include <conio.h>
int frameutils::Getch() { return _getch(); }
#elif defined(__unix__)
/**
 * @ref https://blog.csdn.net/gaopu12345/article/details/30467099
 */
int frameutils::Getch() {
    char c;
    system("stty -echo");   // no echo
    system("stty -icanon"); // read immediately
    c = getchar();
    system("stty icanon");
    system("stty echo"); // cancel settings
    return c;
}
#else

int frameutils::Getch() {
    return getchar(); // an enter is needed when input
}

#endif

void frameutils::Clear() {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#elif defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    system("clear");
#else
    cout << "\033c";
#endif
}

bool frameutils::Confirm(const string &prompt) {
    if (!prompt.empty())
        cout << prompt;
    string str;
    while (1) {
        getline(std::cin, str);
        string &&trimedStr = Trim(str);
        if (trimedStr.empty())
            continue;
        if (trimedStr[0] == 'y')
            return true;
        if (trimedStr[0] == 'n')
            return false;
    }
}

int frameutils::Option(const std::vector<string> &options, const string &prompt,
                       const std::vector<string> &inputList) {
    int op;
    for (int i = 1; i <= options.size(); i++) {
        cout << i << ". " << options[i - 1] << std::endl;
    }
    cout << std::endl;
    if (!prompt.empty())
        cout << prompt;
    for (;;) {
        string str;
        std::cin >> str;
        string cmd = Trim(str);
        auto it = std::find(inputList.begin(), inputList.end(), cmd);
        if (it != inputList.end()) {
            return inputList.begin() - it;
        }
        std::istringstream oss(str);
        oss >> op;
        if (op < 1 || op > options.size()) {
            cout << "Invalid command, please try again: ";
        } else
            break;
    }
    return op;
}