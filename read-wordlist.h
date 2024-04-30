#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include <ncurses.h>

#define MAX_WORDS 7775
#define MAX_WORD_LENGTH 100

void exit_program() {
    int ch = getch();
    if (ch == 'q' || ch == 'Q') {
        endwin();
        exit(0);
    }
    else {
        return;
    }
}

int oob(int y, int x) {
    if (y > 24) {
        y = 24;
    }
    if (x > 300) {
        x = 300;
    }
}