#include "../headers/safe_input.h"
#include <stdio.h>
#include <stdlib.h>

float get_safe_float(float min, float max, int max_attempts, const char *message) {

    int temp;
    int counter = 0;
    float x;
    char c;

    while (1) {

        if (counter == max_attempts) {
            printf("\n!FATAL ERROR! Maximum attempts reached. Terminating program!\n");
            getchar();
            exit(1);
        }

        printf("%s", message);
        counter++;

        if (scanf("%f%c", &x, &c) != 2 || (c != '\n' && c != '\r')) {
            printf("\n!ERROR! Invalid input. Please enter a valid number (e.g., 12.5 or -5.0)\n");
            while ((temp = getchar()) != '\n' && temp != EOF)
                ;
            continue;
        }

        if (x < min || x > max) {
            printf("\n!ERROR! Value must be between %.3f and %.3f\n", min, max);
            continue;
        }

        return x;
    }
}
