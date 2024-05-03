#include <stdio.h>
#include <stdlib.h>

int main() {
    int result = system("python3 lol.py");
    if (result != 0) {
        printf("Error: could not run python script\n");
        return 1;
    }

    printf("Thanks for playing!\n");

}