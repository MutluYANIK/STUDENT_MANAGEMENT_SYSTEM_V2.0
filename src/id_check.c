#include "../headers/student_management_system.h"
#include <stdio.h>

int id_check(student *head, unsigned int id) {
    student *current = head;

    while (current != NULL) {
        if (current->id == id) {
            printf("\n!ERROR! A student with ID %u already exists in the list!\n\n", id);
            return 1;
        }
        current = current->next;
    }
    return 0;
}