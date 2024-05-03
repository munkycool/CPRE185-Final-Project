#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ncurses.h>

#define MAX_WORDS 7775
#define MAX_WORD_LENGTH 20

void adjust_positions(char **curWord, int *positions, int width, int *yAxis, int num_words) {
    for (int i = 0; i < num_words; i++) {
        if (positions[i] + strlen(curWord[i]) > width) {
            positions[i] = width - strlen(curWord[i]);
            yAxis[i] = 0;
        }
    }
}

int check_overlap(int new_position, int *existing_positions, int *word_lengths, int num_words, int word_length) {
    for (int i = 0; i < num_words; i++) {
        if ((existing_positions[i] - 1) <= new_position && new_position < (existing_positions[i] + word_lengths[i] + 1)) {
            return 1;
        }
        if ((existing_positions[i] - 1) < (new_position + word_length + 1) && (new_position + word_length + 1) <= (existing_positions[i] + word_lengths[i] + 1)) {
            return 1;
        }
    }
    return 0;
}

int main() {
    initscr();
    curs_set(0);
    
    FILE *fp = fopen("wordlist.txt", "r");
    if (fp == NULL) {
        endwin();
        printf("Error opening wordlist.txt\n");
        return 1;
    }

    // First pass to count the number of words
    int num_words = 0;
    char temp_word[MAX_WORD_LENGTH];
    while (fscanf(fp, "%s", temp_word) != EOF) {
        num_words++;
    }
    rewind(fp); // Go back to the start of the file

    // Allocate memory for the words array based on the number of words
    char **words = malloc(num_words * sizeof(char*));
    if (words == NULL) {
        endwin();
        printf("Memory allocation failed\n");
        fclose(fp);
        return 1;
    }

    // Second pass to actually store the words
    int index = 0;
    while (fscanf(fp, "%s", temp_word) != EOF && index < num_words) {
        words[index] = malloc(strlen(temp_word) + 1);
        if (words[index] == NULL) {
            endwin();
            printf("Memory allocation failed for word %d\n", index);
            // Free previously allocated memory
            for (int i = 0; i < index; i++) {
                free(words[i]);
            }
            free(words);
            fclose(fp);
            return 1;
        }
        strcpy(words[index], temp_word);
        index++;
    }

    fclose(fp);
    
    char *curWord[MAX_WORDS];
    int positions[MAX_WORDS];
    int yAxis[MAX_WORDS];
    int word_lengths[MAX_WORDS];
    
    srand(time(NULL));
    int random_index = rand() % num_words;
    curWord[0] = words[random_index];
    positions[0] = rand() % (COLS - strlen(curWord[0]));
    yAxis[0] = 0;
    word_lengths[0] = strlen(curWord[0]);
    
    time_t start_time = time(NULL);
    
    changeisbeautiful:
    mvprintw(0, 0, "Do you want to change the wordlist? (y/n): ");
    refresh();
    char change_wordlist = getch();
    
    /*
    if (change_wordlist == 'y') {
        endwin();
        system("open -a CotEditor wordlist.txt");
        initscr();
    }
    */

    if (change_wordlist == 'y') {
        char user_input[256]; // Assuming a max word length of 255 characters
        mvprintw(2, 0, "Enter new word to append: ");
        refresh();
        getnstr(user_input, 255); // Read input from user, with a maximum of 255 characters

        // Open the file in append mode
        FILE *fp = fopen("wordlist.txt", "a");
        if (fp != NULL) {
            fprintf(fp, "%s\n", user_input); // Append the new word to the file, followed by a newline
            fclose(fp); // Close the file
        } else {
            mvprintw(3, 0, "Failed to open wordlist.txt for appending.");
        }
        clear(); // Clear the screen after appending
        goto changeisbeautiful;
    }
    
    clear();
    mvprintw(0, 0, "Choose a difficulty level (1-10): ");
    refresh();
    int difficulty;
    scanw("%d", &difficulty);
    clear();
    
    int num_cur_words = 1;
    
    while (1) {
        int existing_positions[MAX_WORDS];
        int existing_word_lengths[MAX_WORDS];
        
        for (int i = 0; i < num_cur_words; i++) {
            existing_positions[i] = positions[i];
            existing_word_lengths[i] = word_lengths[i];
        }
        
        for (int i = 0; i < num_cur_words; i++) {
            int new_position = rand() % (COLS - word_lengths[i]);
            while (check_overlap(new_position, existing_positions, existing_word_lengths, num_cur_words, word_lengths[i])) {
                new_position = rand() % (COLS - word_lengths[i]);
            }
            positions[i] = new_position;
            existing_positions[i] = new_position;
        }
        
        int randIndices[MAX_WORDS];
        int num_rand_indices = (num_cur_words >= difficulty) ? difficulty : num_cur_words;
        for (int i = 0; i < num_rand_indices; i++) {
            randIndices[i] = rand() % num_cur_words;
        }
        
        if (difftime(time(NULL), start_time) > 30) {
            difficulty = 20;
        }
        
        for (int i = 0; i < num_rand_indices; i++) {
            int randIndex = randIndices[i];
            if (difftime(time(NULL), start_time) > 5) {
                yAxis[randIndex] += 2;
            } else if (difftime(time(NULL), start_time) > 10) {
                yAxis[randIndex] += 4;
                yAxis[randIndex] += 4;
            } else if (difftime(time(NULL), start_time) > 15) {
                yAxis[randIndex] += 6;
                yAxis[randIndex] += 6;
            } else if (difftime(time(NULL), start_time) > 20) {
                yAxis[randIndex] += 8;
                yAxis[randIndex] += 8;
            } else if (difftime(time(NULL), start_time) > 25) {
                yAxis[randIndex] += 10;
                yAxis[randIndex] += 10;
            } else if (difftime(time(NULL), start_time) > 30) {
                yAxis[randIndex] += 12;
                yAxis[randIndex] += 12;
            }
        }
        
        int game_over = 0;
        for (int i = 0; i < num_cur_words; i++) {
            if (yAxis[i] >= LINES) {
                game_over = 1;
                break;
            }
        }
        
        if (game_over) {
            break;
        }
        
        for (int i = 0; i < num_cur_words; i++) {
            mvprintw(yAxis[i], positions[i], "%s", curWord[i]);
        }
        
        refresh();
        
        mvprintw(LINES - 1, 0, "Enter a word: ");
        refresh();
        char user_input[MAX_WORD_LENGTH];
        getstr(user_input);
        
        if (strcmp(user_input, ":q") == 0) {
            break;
        }
        
        int found = 0;
        for (int i = 0; i < num_cur_words; i++) {
            if (strcmp(user_input, curWord[i]) == 0) {
                mvprintw(yAxis[i], positions[i], "%*s", word_lengths[i], " ");
                free(curWord[i]);
                for (int j = i; j < num_cur_words - 1; j++) {
                    curWord[j] = curWord[j + 1];
                    positions[j] = positions[j + 1];
                    yAxis[j] = yAxis[j + 1];
                    word_lengths[j] = word_lengths[j + 1];
                }
                num_cur_words--;
                found = 1;
                break;
            }
        }
        
        if (found) {
            if (num_cur_words < 10) {
                random_index = rand() % num_words;
                curWord[num_cur_words] = words[random_index];
                positions[num_cur_words] = 20 * num_cur_words;
                yAxis[num_cur_words] = 0;
                word_lengths[num_cur_words] = strlen(curWord[num_cur_words]);
                num_cur_words++;
            }
            
            random_index = rand() % num_words;
            curWord[num_cur_words] = words[random_index];
            positions[num_cur_words] = rand() % (COLS - strlen(curWord[num_cur_words]));
            yAxis[num_cur_words] = 0;
            word_lengths[num_cur_words] = strlen(curWord[num_cur_words]);
            num_cur_words++;
        }
        
        getch();
        clear();
    }
    
    clear();
    mvprintw(1, 0, "Time taken: %.2f seconds", difftime(time(NULL), start_time));
    mvprintw(0, 0, "Game Over!");
    refresh();
    sleep(5);
    endwin();
    
    for (int i = 0; i < num_words; i++) {
        free(words[i]);
    }
    
    return 0;
}
