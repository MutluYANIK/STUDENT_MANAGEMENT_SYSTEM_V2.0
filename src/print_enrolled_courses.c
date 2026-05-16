#include "../headers/student_management_system.h"
#include <stdio.h>

void print_enrolled_courses(student *current_student){

    enrollment *current_enrollment = current_student->records;

    printf("\n\n");
    
    while(current_enrollment != NULL){
        
        printf("%s\n",current_enrollment->course->course_id);
        current_enrollment = current_enrollment->next;
    }
}