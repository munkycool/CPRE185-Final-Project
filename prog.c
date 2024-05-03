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
// Hello
    char *words[MAX_WORDS];
    int num_words = 0;
    srand(time(NULL));
    char *cur_word[10];
    int random_number = rand() % MAX_WORDS + 1;
    int y_axis[10], x_axis[10];
    time_t start_time1, start_time2, end_time;
    int i, j, k, n = 0;
    char input[100];
    int input_index = 0;
    int ch;

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
            random_number = rand() % MAX_WORDS + 1;
            start_time1 = time(NULL);
            strcpy(cur_word[i], words[random_number]);
            mvprintw(y_axis[i], x_axis[i], cur_word[i]);
            x_axis[i] += 15 + x_axis[i-1];
            i++;
            refresh();
        }

/*
        if (time(NULL) - start_time2 >= 2) {
            for (j = 0; j < 10; j++) {
                if (cur_word[j][0] == '\0') {
                    continue;
                }
                y_axis[j]++;
                mvprintw(y_axis[j], x_axis[j], "%s", cur_word[j]);
                refresh();
            }
        }
*/
        for (j = 0; j < 10; j++) {
            ch = getch();
            if (ch != ERR) {  // if a key was pressed
                if (ch == '\n') {  // if the key was Enter
                    input[input_index] = '\0';  // null-terminate the string
                    // ... (compare input to cur_word[j] and update as before)
                    input_index = 0;  // reset the input index for the next word
                } else if (ch == KEY_BACKSPACE && input_index > 0) {  // if the key was Backspace
                    input_index--;  // decrement the input index
                    input[input_index] = '\0';  // null-terminate the string
                } else if (input_index < MAX_WORD_LENGTH) {  // if there's room for more characters
                    input[input_index] = ch;  // append the character to the string
                    input_index++;  // increment the input index
                }
                mvprintw(LINES - 1, 0, "%s", input);  // display the input at the bottom of the screen
                clrtoeol();  // clear the rest of the line
                refresh();
            }
}
}
}