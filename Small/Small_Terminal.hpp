#ifndef SMALL_TERMINAL
#define SMALL_TERMINAL

#include <iostream>

namespace terminal {
    void move(unsigned int x, unsigned int y) {
        printf("\033[%d;%df", x, y);
    }

    void fcolor(unsigned char r, unsigned char g, unsigned char b) {
        printf("\033[38;2;%d;%d;%dm",r,g,b);
    }

    void bcolor(unsigned char r, unsigned char g, unsigned char b) {
        printf("\033[48;2;%d;%d;%dm",r,g,b);
    }

    void color(unsigned char fr, unsigned char fg, unsigned char fb,
               unsigned char br, unsigned char bg, unsigned char bb) {
        fcolor(fr,fg,fb);
        bcolor(br,bg,bb);
    }

    void reset() {
        printf("\033[0m");
    }
}

#endif