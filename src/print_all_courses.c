#include "../headers/student_management_system.h"
#include <stdio.h>

void print_all_courses(course *course_head){

    course *current_course = course_head;
    
    while(current_course != NULL){
        
        printf("%s\n", current_course->course_id);
        current_course = current_course->next;
    }
}