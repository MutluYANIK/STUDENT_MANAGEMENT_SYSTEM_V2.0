#include "student_management_system.h"
#include <string.h>

int lecture_id_check(lecture *lecture_head, const char *lecture_id) {

    lecture *current = lecture_head;
    
    while (current != NULL) {
        if (strcmp(current->lecture_id, lecture_id) == 0) {
            printf("\n!ERROR! Lecture ID already exists!");
            return 1;
        }
        current = current->next;
    }

    return 0;