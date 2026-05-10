#include "../headers/student_management_system.h"
#include <stdio.h>

void print_all_lectures(lecture *lecture_head){

    lecture *current_lecture = lecture_head;

    while(current_lecture != NULL){
        
        printf("%s\n", current_lecture->lecture_id);
        current_lecture = current_lecture->next;
    }
}