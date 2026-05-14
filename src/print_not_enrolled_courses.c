#include "../headers/student_management_system.h"
#include <stdio.h>

void print_not_enrolled_courses(student *current_student, lecture *course_head){

    enrollment *current_enrollment = current_student->records;
    lecture *current_course = course_head;

    while(current_course != NULL){

        while(current_enrollment != NULL){

            if(strcmp(current_enrollment->lecture->lecture_id, current_course->lecture_id) != 0){
                printf("%s\n", current_course->lecture_id);
            }

            current_enrollment = current_enrollment->next;
        }

        current_course = current_course->next;
    }
}