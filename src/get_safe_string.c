#include "../headers/safe_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void get_safe_string(int max_attempts, int max_length, char *target, const char *message) {

    int attempts = 0;

    while (1) {

        // Prevents infinite error loop in case of continuous invalid inputs or automated spam
        if (attempts >= max_attempts) {
            printf("\n!FATAL ERROR! Maximum attempts reached! Terminating...\n\n");
            exit(1);
        }

        attempts++;

        printf("%s", message);

        // Reads input safely up to the buffer limit to prevent buffer overflow
        if (fgets(target, max_length, stdin) != NULL) {
            size_t length = strlen(target);
            
            // Strips the trailing newline ('\n') captured by fgets to ensure clean string formatting
            if (length > 0 && target[length - 1] == '\n') {
                target[length - 1] = '\0';
                length--;
            } else {
                int c;
                while ((c = getchar()) != '\n' && c != EOF)
                    ;
            }

            if (length > 0) {
                return;
            } else {
                printf("\n!ERROR! Input cannot be empty. Please try again.\n\n");
            }
        }
    }
}