#include "../headers/safe_input.h"
#include <stdio.h>
#include <stdlib.h>

int get_safe_int_between(int min, int max, int max_attempts, const char *message) {

    int x, temp;
    int counter = 0;
    char c;

    while (1) {

        // Prevents infinite error loop in case of continuous invalid inputs or automated spam
        if (counter == max_attempts) {
            printf("\n!FATAL ERROR! Maximum attempts reached. Terminating "
                   "program!\n\n");
            getchar();
            exit(1);
        }

        printf("%s", message);
        counter++;

        /*
         * Ensures the input is EXCLUSIVELY an integer
         * Checks for the Enter key ('\n' or '\r') immediately after the number
         * prevents trailing garbage characters (e.g., "123abc" or "12.5") from slipping through.
         */
        if (scanf("%d%c", &x, &c) != 2 || (c != '\n' && c != '\r')) {
            printf("\n!ERROR! You must enter only an integer (FLOAT OR "
                   "CHARACTERS NOT ALLOWED!)\n\n");
            while ((temp = getchar()) != '\n' && temp != EOF)
                ;
            continue;
        }

        // Ensures the number is within the desired range
        if (x < min || x > max) {
            printf("\n!ERROR! Value must be between %d and %d\n\n", min, max);
            continue;
        }

        return x;
    }
}