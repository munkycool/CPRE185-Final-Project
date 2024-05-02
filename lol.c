#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORDS 10
#define MAX_WORD_LENGTH 50
#define MAX_LINE_LENGTH 200

int main() {
    initscr();
    clear();
    curs_set(0);

    FILE *file = fopen("wordlist.txt", "r");
    if (file == NULL) {
        endwin();
        return 1;
    }

    // Count the number of lines (words) in the file
    int wordCount = 0;
    char ch;
    while (!feof(file)) {
        ch = fgetc(file);
        if (ch == '\n') {
            wordCount++;
        }
    }

    // Allocate memory for the words array
    char (*words)[MAX_WORD_LENGTH] = malloc(wordCount * sizeof(*words));
    if (words == NULL) {
        endwin();
        return 1;
    }

    // Go back to the start of the file
    rewind(file);

    // Read words from file
    char line[MAX_LINE_LENGTH];
    int i = 0;
    while (fgets(line, sizeof(line), file)) {
        strcpy(words[i], line);
        i++;
    }
    fclose(file);

    int yAxis[MAX_WORDS];
    int positions[MAX_WORDS];
    char curWord[MAX_WORDS][MAX_WORD_LENGTH];

    // Initialize yAxis and positions
    for (int i = 0; i < MAX_WORDS; i++) {
        yAxis[i] = 0;
        positions[i] = 20 * i;
    }

    // Select random words
    srand(time(NULL));
    for (int i = 0; i < MAX_WORDS; i++) {
        int randIndex = rand() % wordCount;
        strcpy(curWord[i], words[randIndex]);
    }

    time_t start_time = time(NULL);

    while (1) {
        // Reset positions
        for (int i = 0; i < MAX_WORDS; i++) {
            positions[i] = 20 * i;
        }

        // Move a random word one position down
        int randIndex = rand() % MAX_WORDS;
        if (time(NULL) - start_time > 5) {
            yAxis[randIndex] += 1;
        }

        // Check if any word has reached the bottom of the screen
        int height, width;
        getmaxyx(stdscr, height, width);
        for (int i = 0; i < MAX_WORDS; i++) {
            if (yAxis[i] >= height) {
                endwin();
                free(words);
                return 0;
            }
        }

        // Display words
        for (int i = 0; i < MAX_WORDS; i++) {
            mvprintw(yAxis[i], positions[i], curWord[i]);
        }

        refresh();

        // Get user input
        char userInput[MAX_WORD_LENGTH];
        mvprintw(10, 0, "Enter a word: ");
        getstr(userInput);

        // Clear word if user input matches
        for (int i = 0; i < MAX_WORDS; i++) {
            if (strcmp(userInput, curWord[i]) == 0) {
                for (int j = 0; j < strlen(userInput); j++) {
                    mvaddch(yAxis[i], positions[i] + j, ' ');
                }
            }
        }

        refresh();
        getch();  // Wait for user input before continuing

        clear();
    }

    endwin();
    free(words);
    return 0;
}