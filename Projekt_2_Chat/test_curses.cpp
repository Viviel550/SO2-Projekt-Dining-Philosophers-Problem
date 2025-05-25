#include <ncurses/ncurses.h>
#include <iostream>

int main() {
    initscr();
    printw("Hello NCurses World!");
    refresh();
    getch();
    endwin();
    return 0;
}