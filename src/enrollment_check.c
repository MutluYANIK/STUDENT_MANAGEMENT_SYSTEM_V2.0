#include "../headers/safe_input.h"
#include "../headers/student_management_system.h"
#include <stdio.h>

int enrollment_check(student *current_student, course *target_course) {

    enrollment *temp = current_student->records;

    while (temp != NULL) {
        if (temp->course == target_course) {
            return 1; // Student is already enrolled in the target course
        }
        temp = temp->next;
    }
    return 0; // Student is not enrolled in the target course
}
