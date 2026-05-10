#include "../headers/student_management_system.h"
#include <stdio.h>
#include <string.h>

lecture *find_lecture(lecture *lecture_head, const char *target_lecture_id){

    lecture *current_lecture = lecture_head;

    while(current_lecture != NULL){
        
        if(strcmp(current_lecture->lecture_id, target_lecture_id) == 0){
            break;
        }

        current_lecture = current_lecture->next;
    }

    if(current_lecture == NULL){
        printf("\n!ERROR! This lecture id '%s' not exist in the lecture list\n\n", target_lecture_id);
        return NULL;
    }

    return current_lecture;
}