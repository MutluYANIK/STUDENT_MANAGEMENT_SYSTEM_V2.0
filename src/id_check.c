#include "../headers/student_management_system.h"
#include <stdio.h>

int id_check(student *head, unsigned int id) {
    student *current = head;

    while (current != NULL) {
        if (current->id == id) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}