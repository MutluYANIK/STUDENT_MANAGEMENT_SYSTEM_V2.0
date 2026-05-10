#include "../headers/student_management_system.h"
#include <stdio.h>

void print_not_enrolled_lectures(student *current_student, lecture *lecture_head){

    enrollment *current_enrollment = current_student->records;
    lecture *current_lecture = lecture_head;

    while(current_lecture != NULL){

        while(current_enrollment != NULL){

            if(strcmp(current_enrollment->lecture->lecture_id, current_lecture->lecture_id) != 0){
                printf("%s\n", current_lecture->lecture_id);
            }

            current_enrollment = current_enrollment->next;
        }

        current_lecture = current_lecture->next;
    }
}