#include "../headers/safe_input.h"
#include "../headers/student_management_system.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int add_student(student **student_head, lecture *lecture_head) { 

    /*
     * Gives you the chance to exit the function while taking student ID input, checks for duplicate
     * student IDs to maintain data integrity and prevent confusion in the system.
     */
    unsigned int temp = get_safe_unsigned_int(3, "Enter student ID (or 0 to return to main menu): ");

    if (temp == 0) {
        printf("\nOperation cancelled. Returning to menu...\n\n");
        return 0;
    }

    if (id_check(*student_head, temp) == 1) {
        return 1;
    }

    // Takes input for the student name and converts it to uppercase for consistency.
    char temp_student_name[40];
    get_safe_name(3, temp_student_name, sizeof(temp_student_name), "\nEnter student name: ");

    for (int i = 0; temp_student_name[i] != '\0'; i++) {
        temp_student_name[i] = toupper((unsigned char)temp_student_name[i]);
    }

    student *new_student = (student *)malloc(sizeof(student));

    if (new_student == NULL) {
        printf("\n!ERROR! Memory allocation failed!\n\n");
        return 0;
    }

    new_student->id = temp;
    strcpy(new_student->name, temp_student_name);   



    return 1;
}