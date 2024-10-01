#include "cmdframe.h"
#include "frameutils.h"
#include "ncursesstream.h"
#include <iostream>
#include <ncurses.h>

using std::cout;

finf_id_t CmdFrame::Execute() const {
    // 1. init
    std::streambuf *cout_buf{nullptr};
    if (this->init_type == InitType::CLEAR)
        frameutils::Clear();
    else if (this->init_type == InitType::CURSE) {
        WINDOW *stdscr = initscr();
        // redirect cout to ncurses stdscr
        NcursesStream ncursesOut(stdscr);
        cout_buf = std::cout.rdbuf(); // save original cout buf
        std::cout.rdbuf(ncursesOut.rdbuf());
    }
    // 2. frame main
    int ret = this->frame_main();
    // 3. end init
    if (this->init_type == InitType::CURSE) {
        std::cout.rdbuf(cout_buf);
        endwin();
    }
    return ret;
}
