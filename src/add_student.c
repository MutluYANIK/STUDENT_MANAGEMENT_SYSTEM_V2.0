#include "../headers/safe_input.h"
#include "../headers/student_management_system.h"
#include <stdio.h>
#include <stdlib.h>

int add_student(student **student_head) {

    unsigned int temp = get_safe_unsigned_int(3, "Enter student ID (or 0 to return to main menu): ");

    if (temp == 0) {
        printf("\nOperation cancelled. Returning to menu...\n\n");
        return 0;
    }

    if (id_check(*student_head, temp) == 1) {

        return 1;
    }

    return 1;
}