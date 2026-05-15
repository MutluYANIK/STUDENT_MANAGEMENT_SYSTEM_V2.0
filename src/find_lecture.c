#include "../headers/student_management_system.h"
#include <stdio.h>
#include <string.h>

course *find_course(course *course_head, const char *target_course_id){

    course *current_course = course_head;

    while(current_course != NULL){
        
        if(strcmp(current_course->course_id, target_course_id) == 0){
            break;
        }

        current_course = current_course->next;
    }

    if(current_course == NULL){
        printf("\n!ERROR! This course id '%s' not exist in the course list\n\n", target_course_id);
        return NULL;
    }

    return current_course;
}