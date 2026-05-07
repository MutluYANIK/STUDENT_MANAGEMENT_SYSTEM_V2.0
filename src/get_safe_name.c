#include "../headers/safe_input.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void get_safe_name(int max_attempts, char *target, int max_length, const char *message) {

    int attempts = 0;

    while (1) {

        // Prevents infinite error loop in case of continuous invalid inputs or automated spam
        if (attempts >= max_attempts) {
            printf("\n!FATAL ERROR! Maximum attempts reached for name input. "
                   "Terminating...\n\n");
            exit(1);
        }

        attempts++;

        int has_letter = 0;
        int is_valid = 1;

        printf("%s", message);

        // Reads input safely up to the buffer limit to prevent buffer overflow
        if (fgets(target, max_length, stdin) == NULL) {
            continue;
        }

        size_t length = strlen(target);

        // Strips the trailing newline ('\n') captured by fgers to ensure clean string formatting
        if (length > 0 && target[length - 1] == '\n') {
            target[length - 1] = '\0';
        }
        /*
         * Flushes leftover character in stdin if the input exceeded max_length, preventing them from
         * bleeding into the next prompt
         */
        else {
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
        }

        // Rejects empty inputs caused by accidental Enter key presses
        if (strlen(target) == 0) {
            printf("\n!ERROR! Input cannot be empty. Please try again.\n\n");
            continue;
        }

        // Enforces strict naming rules: restricts input to letters and spaces, and rejects space-only strings
        for (int i = 0; target[i] != '\0'; i++) {
            if (isalpha((unsigned char)target[i])) {
                has_letter = 1;
            } else if (target[i] != ' ') {
                is_valid = 0;
                break;
            }
        }

        /*
         * Evaluates validation flags to provide targeted error feedback, or safely breaks the loop if
         * the input is perfect
         */
        if (!is_valid) {
            printf("\n!ERROR! Name can only contain letters and spaces.\n\n");
        } else if (!has_letter) {
            printf("\n!ERROR! Name must contain at least one letter.\n\n");
        } else
            break;
    }
}