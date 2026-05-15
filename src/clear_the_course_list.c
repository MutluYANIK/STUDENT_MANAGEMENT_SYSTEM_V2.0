#include "../headers/safe_input.h"
#include "../headers/student_management_system.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Checks the course list, if it is already empty prints an error message and returns previous menu. Takes
 * confirmation from the user before clearing the course list. Clears all students' scores and records,
 * updates their GPA and clears the course list.
 */

void clear_the_course_list(student *student_head, course **course_head) {

    student *current_student = student_head;

    while (current_student != NULL) {

        enrollment *current_enrollment = current_student->records;
        enrollment *previous_enrollment = NULL;

        while (current_enrollment != NULL) {

            previous_enrollment = current_enrollment;
            current_enrollment = current_enrollment->next;

            free(previous_enrollment->scores);
            free(previous_enrollment);
        }

        current_student->records = NULL;
        calculate_student_gpa(current_student);
        current_student = current_student->next;
    }

    course *current_course = *course_head;
    course *previous_course = NULL;

    while (current_course != NULL) {

        exam_template *current_exam = current_course->exams;
        exam_template *previous_exam = NULL;

        while (current_exam != NULL) {

            previous_exam = current_exam;
            current_exam = current_exam->next;
            free(previous_exam);
        }

        previous_course = current_course;
        current_course = current_course->next;

        free(previous_course);
    }

    *course_head = NULL;

    printf("\nCourse list successfully cleared\n\n");

    student_save_flag = 1;
    course_save_flag = 1;
}