#include "../headers/safe_input.h"
#include "../headers/student_management_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int add_lecture(lecture **lecture_head) {
    /*
     * gives you the chance to exit the function while taking lecture id input, checks for duplicate
     * lecture ids to maintain data integrity and prevent confusion in the system.
     */
    char temp[10];
    get_safe_name(3, temp, sizeof(temp), "Enter lecture ID (or 0 to return to main menu): ");

    if (strcmp(temp, "0") == 0) {
        printf("\nOperation cancelled. Returning to menu...\n\n");
        return 0;
    }

    if (lecture_id_check(*lecture_head, temp) == 1) {
        return 1;
    }
    /*
     * Allocates memory for a new lecture and checks for successful allocation to prevent crashes.
     */
    lecture *new_lecture = (lecture *)malloc(sizeof(lecture));

    if (new_lecture == NULL) {
        printf("\n!ERROR! Memory allocation failed!\n\n");
        return 0;
    }

    strcpy(new_lecture->lecture_id, temp);
    /*
     * Takes input for the lecture name and converts it to uppercase for consistency.
     */
    char lecture_name[40];
    get_safe_name(3, lecture_name, sizeof(lecture_name), "Enter lecture name: ");

    for (int i = 0; lecture_name[i] != '\0'; i++) {
        lecture_name[i] = toupper((unsigned char)lecture_name[i]);
    }

    strcpy(new_lecture->lecture_name, lecture_name);

    }
}