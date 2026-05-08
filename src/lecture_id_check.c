#include "student_management_system.h"
#include <string.h>

int lecture_id_check(lecture *lecture_head, const char *id1) {

    lecture *current = lecture_head;

    while (current != NULL) {
        if (strcmp(current->lecture_id, id1) == 0) {
            printf("\n!ERROR! Lecture ID alread exists!");
            return 1;
        }
        current = current->next;
    }

    return 0;