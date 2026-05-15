#include "../headers/student_management_system.h"
#include <stdio.h>

void print_all_courses(lecture *course_head){

    lecture *current_course = course_head;

    printf("\n\n");
    
    while(current_course != NULL){
        
        printf("%s\n", current_course->lecture_id);
        current_course = current_course->next;
    }
}