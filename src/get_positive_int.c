#include "../headers/safe_input.h"
#include <stdio.h>
#include <stdlib.h>

int get_positive_int(int max_attempts, const char *message) {

    int x, temp;
    int counter = 0;
    char c;

    while (1) {

        if (counter == max_attempts) {
            printf("\n!FATAL ERROR! Maximum attempts reached. Terminating program!\n");
            getchar();
            exit(1);
        }

        printf("%s", message);
        counter++;

        if (scanf("%d%c", &x, &c) != 2 || (c != '\n' && c != '\r')) {
            printf("\nERROR! You must enter only a positive integer (FLOAT OR CHARACTERS NOT ALLOWED!)\n");
            while ((temp = getchar()) != '\n' && temp != EOF)
                ;
            continue;
        }

        if (x <= 0) {
            printf("\nERROR! You must enter only a positive integer\n");
            continue;
        }

        return x;
    }
}