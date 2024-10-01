#include "cmdframe.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

using std::cout;

CmdFrame::CmdFrame() {}

CmdFrame::~CmdFrame() {}

int CmdFrame::Execute() const {
    return this->frame_main();
}





