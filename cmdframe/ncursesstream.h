#ifndef __NCURSESSTREAM_H__
#define __NCURSESSTREAM_H__
#include <iostream>
#include <ncurses.h>

class NcursesStream : public std::ostream {
  public:
    NcursesStream(WINDOW *ncursesWindow)
        : std::ostream(&buffer), buffer(ncursesWindow) {}

  private:
    class NcursesBuffer : public std::streambuf {
      public:
        NcursesBuffer(WINDOW *ncursesWindow) : window(ncursesWindow) {}

      protected:
        virtual int overflow(int c) override {
            if (c != EOF) {
                waddch(window, c);
            }
            return c;
        }

      private:
        WINDOW *window;
    } buffer;
};

#endif