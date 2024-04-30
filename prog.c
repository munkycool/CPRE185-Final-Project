/*
Copyright (c) 2024 Artemis Ulysses and Ian Anton 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "read-wordlist.h"
#include <ncurses.h>

int main () {

    char *words[MAX_WORDS];
    int num_words = 0;
    srand(time(NULL));
    char *cur_word[10];
    int random_number = rand() % MAX_WORDS + 1;
    int y_axis[10], x_axis[10];
    time_t start_time1, start_time2, end_time;
    int i, j, k, n = 0;
    char input[100];

// read the words from the wordlist.txt file

    FILE *file = fopen("wordlist.txt", "r");
    if (file == NULL) {
        printf("Error: could not open file\n");
        return 1;
    }

    for (int i = 0; i < MAX_WORDS; i++) {
        words [i] = malloc(MAX_WORD_LENGTH + 1);
        if (words[i] == NULL) {
            printf("Error: out of memory\n");
            return 1;
        }
        if (fscanf(file, "%99s", words[i]) != 1) {
            break;
        }
        num_words++;
    }

    fclose(file);

    initscr();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);

    for (i = 0; i < 10; i++) {
        cur_word[i] = calloc(1, MAX_WORD_LENGTH + 1);
    }

    i = 0;

    while (1) {
        exit_program();

        if (time(NULL) - start_time1 >= 1) {
            if (i == 10) {
                i = 0;
            }
            if (cur_word[i][0] != '\0') {
                continue;
            }
            random_number = rand() % MAX_WORDS + 1;
            start_time1 = time(NULL);
            strcpy(cur_word[i], words[random_number]);
            mvprintw(y_axis[i], x_axis[i], cur_word[i]);
            x_axis[i] += 20 + x_axis[i-1];
            i++;
            refresh();
        }
        if (time(NULL) - start_time2 >= 2) {
            int word_to_move = rand() % 10;
            start_time2 = time(NULL);
            oob(y_axis[word_to_move], x_axis[word_to_move]);
            y_axis[word_to_move]++;
            mvprintw(y_axis[word_to_move], x_axis[word_to_move], "%s", cur_word[word_to_move]);
            refresh();
        }

        for (j = 0; j < 10; j++) {
            scanw("%s", input);
            if (strcmp(input, cur_word[j]) == 0) {
                y_axis[j] = 0;
                x_axis[j] = rand() % 300;
                cur_word[j][0] = '\0';
                mvprintw(y_axis[j], x_axis[j], "%s", cur_word[j]);
                refresh();
            }
        }
}
}