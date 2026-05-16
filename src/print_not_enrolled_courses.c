#include "../headers/student_management_system.h"
#include <stdio.h>
#include <string.h>

void print_not_enrolled_courses(student *current_student, course *course_head) {

    course *current_course = course_head;

    if(current_course == NULL){
        printf("\nNo courses available in the system\n");
        return;
    }

    while (current_course != NULL) {

        int enrolled_flag = 0;
        enrollment *current_enrollment = current_student->records;

        while (current_enrollment != NULL) {

            if (strcmp(current_enrollment->course->course_id, current_course->course_id) == 0) {
                enrolled_flag = 1;
                break;
            }

            current_enrollment = current_enrollment->next;
        }

        if (!enrolled_flag) {
            printf("%s\n", current_course->course_id);
        }
        current_course = current_course->next;
    }

    printf("\n\n");
}