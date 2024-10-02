#include "cmdframe_manager.h"
#include <algorithm>
#include <iostream>
#include <stack>

using std::cerr;
using std::cout;
using std::endl;

void CmdFrameManager::Append(CmdFrame *frame) {
    frames.push_back(frame);
    frame_keys.push_back("");
}
void CmdFrameManager::Append(const std::string &name, CmdFrame *frame) {
    frames.push_back(frame);
    if (fkey_to_id.count(name) != 0) {
        cerr << "Frame key [" << name << "] already exists" << std::endl;
        throw std::runtime_error("Frame key [" + name + "] already exists");
    }
    frame_keys.push_back(name);
    fkey_to_id[name] = frames.size() - 1;
}
void CmdFrameManager::Append(std::vector<CmdFrame *> frames) {
    for (auto &&frame : frames) {
        this->frames.push_back(frame);
        frame_keys.push_back("");
    }
}
void CmdFrameManager::Append(
    std::vector<std::pair<std::string, CmdFrame *>> frames) {
    for (auto &&frame : frames) {
        Append(frame.first, frame.second);
    }
}

void CmdFrameManager::SetFrameKey(frame_id_t fid, const std::string &key) {
    if (fkey_to_id.count(key) != 0) {
        cerr << "Frame key [" << key << "] already exists" << std::endl;
        throw std::runtime_error("Frame key [" + key + "] already exists");
    }
    fkey_to_id[key] = fid;
    frame_keys[fid] = key;
}

void CmdFrameManager::SetEnterFrameID(const std::string &key) {
    if (fkey_to_id.count(key) == 0) {
        cerr << "Frame key [" << key << "] not found" << std::endl;
        throw std::runtime_error("Frame key [" + key + "] not found");
    }
    enter_frame_id = fkey_to_id[key];
}

void CmdFrameManager::Link(std::string &&fkey, finf_id_t finfid,
                           frame_id_t tfid) {
    if (fkey_to_id.count(fkey) == 0) {
        cerr << "Frame key [" << fkey << "] not found" << std::endl;
        throw std::runtime_error("Frame key [" + fkey + "] not found");
    }
    Link(fkey_to_id[fkey], finfid, tfid);
}

void CmdFrameManager::Link(frame_id_t ffid, finf_id_t finfid,
                           std::string &&tkey) {
    if (fkey_to_id.count(tkey) == 0) {
        cerr << "Frame key [" << tkey << "] not found" << std::endl;
        throw std::runtime_error("Frame key [" + tkey + "] not found");
    }
    Link(ffid, finfid, fkey_to_id[tkey]);
}

void CmdFrameManager::Link(std::string &&fkey, finf_id_t finfid,
                           std::string &&tkey) {
    if (fkey_to_id.count(fkey) == 0) {
        cerr << "Frame key [" << fkey << "] not found" << std::endl;
        throw std::runtime_error("Frame key [" + fkey + "] not found");
    }
    if (fkey_to_id.count(tkey) == 0) {
        cerr << "Frame key [" << tkey << "] not found" << std::endl;
        throw std::runtime_error("Frame key [" + tkey + "] not found");
    }
    Link(fkey_to_id[fkey], finfid, fkey_to_id[tkey]);
}

void CmdFrameManager::Execute() {
    std::stack<frame_id_t> frame_stack;
    frame_stack.push(enter_frame_id);
    while (!frame_stack.empty()) {
        frame_id_t cur_frame_id = frame_stack.top();
        finf_id_t interface_id = frames[cur_frame_id]->Execute();
        // Special interface
        if (interface_id == FINF_BACK) {
            frame_stack.pop();
            continue;
        } else if (interface_id == FINF_SELF) {
            continue;
        } else if (interface_id == FINF_EXIT) {
            break;
        }
        // Normal interface
        if (links[cur_frame_id].count(interface_id) == 0) {
            cerr << "Frame [" << cur_frame_id << "] has no link on ["
                 << cur_frame_id << "]" << std::endl;
            throw std::runtime_error("Frame [" + std::to_string(cur_frame_id) +
                                     "] does not have link on [" +
                                     std::to_string(cur_frame_id) + "]");
        }
        frame_id_t next_frame_id = links[cur_frame_id][interface_id];
        // Special frame id
        if (next_frame_id==FID_BACK) {
            frame_stack.pop();
            continue;
        } else if (next_frame_id==FID_EXIT) {
            break;
        }
        // Normal frame id
        frame_stack.push(next_frame_id);
    }
}

// Demo
// #define __CMDFRAME_DEMO__
#ifdef __CMDFRAME_DEMO__
int main() {
    int numAdditions = 0;
    // Define Main menu
    CmdMenu mainMenu;
    // Item 1: Print string "Hello World!"
    mainMenu.Append("print", "Print Hello World", []() -> string {
        cout << "Hello World!" << std::endl;
        CmdFrame::Pause();
        return "";
    });
    // Item 2: Calculate A+B. Switch to Frame addFrame
    mainMenu.Append({"add", "Cal A+B", "addition"});
    // Item 3: Exit program
    mainMenu.Append({"exit", "Exit", cmdf::EXIT_KEY});
    // Title: Recording number of additions performed.
    mainMenu.SetTitle("{{numAdd}} additions performed.", {{"numAdd", "0"}});
    // Description: Manupulation specification
    mainMenu.SetDesc(
        "Press Up or Down Arrow to move cursor and Enter to check.");
    // Define Frame performing addition
    CmdFrame addFrame(
        [&]() -> void {
            int a, b;
            std::cin >> a >> b;
            cout << a + b << std::endl;
            numAdditions++;
            mainMenu.SetArg("numAdd", numAdditions);
            CmdFrame::Pause();
        },
        cmdf::BACK_KEY);
    // Construct a frame manager
    CmdFrameManager manager(
        {{cmdf::ENTER_KEY, &mainMenu}, {"addition", &addFrame}});
    // Execute frames by means of the manager
    manager.Execute();
    return 0;
}
#endif
