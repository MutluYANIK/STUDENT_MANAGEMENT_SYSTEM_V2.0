#include "../headers/safe_input.h"
#include "../headers/student_management_system.h"
#include <stdio.h>

int enrollment_check(student *current_student, lecture *target_lecture) {

    enrollment *temp = current_student->records;

    while (temp != NULL) {
        if (temp->lecture == target_lecture) {
            return 1; // Student is already enrolled in the target lecture
        }
        temp = temp->next;
    }
    return 0; // Student is not enrolled in the target lecture
}
