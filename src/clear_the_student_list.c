#include "../headers/safe_input.h"
#include "../headers/student_management_system.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Checks the student list. If it is empty, it prints an error message and returns to the previous menu. Asks
 * for confirmation before clearing the entire student list. Iterates through all students, freeing the
 * allocated memory for their scores and enrollments, and finally frees each student structure, setting the
 * student list head to NULL.
 */
void clear_the_student_list(student **student_head) {

    student *current_student = *student_head;
    student *previous_student = NULL;

    while (current_student != NULL) {

        enrollment *current_enrollment = current_student->records;
        enrollment *previous_enrollment = NULL;

        while (current_enrollment != NULL) {

            previous_enrollment = current_enrollment;
            current_enrollment = current_enrollment->next;

            free(previous_enrollment->scores);
            free(previous_enrollment);
        }

        previous_student = current_student;
        current_student = current_student->next;

        free(previous_student);
    }

    *student_head = NULL;
    printf("\nStudent list successfully cleared\n\n");
}