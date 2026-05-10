#include "../headers/student_management_system.h"
#include <string.h>
#include <stdio.h>

int is_lecture_id_exist(lecture *lecture_head, const char *lecture_id) {

    lecture *current = lecture_head;
    
    while (current != NULL) {
        if (strcmp(current->lecture_id, lecture_id) == 0) {
            return 1;
        }
        current = current->next;
    }

    return 0;
}