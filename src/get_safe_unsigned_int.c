#include "../headers/safe_input.h"
#include <stdio.h>
#include <stdlib.h>

unsigned int get_safe_unsigned_int(int max_attempts, const char *message) {

    long long x;
    int temp;
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
        if (scanf("%lld%c", &x, &c) != 2 || (c != '\n' && c != '\r')) {
            printf("\n!ERROR! You must enter only an integer (FLOAT OR "
                   "CHARACTERS NOT ALLOWED!)\n\n");
            while ((temp = getchar()) != '\n' && temp != EOF)
                ;
            continue;
        }

        // Rejects negative numbers to prevent silent underflow (wrap-around) when casting to unsigned int
        if (x < 0) {
            printf("\nERROR! You must enter only a positive integer\n\n");
            continue;
        }

        /*
         * Protects against integer overflow by verifying the value fits safely within the standard
         * 32-bit unsigned int limit (UINT_MAX)
         */
        if (x > 4294967295LL) {
            printf("\nERROR! The number entered is too large!\n\n");
            continue;
        }

        return (unsigned int)x;
    }
}