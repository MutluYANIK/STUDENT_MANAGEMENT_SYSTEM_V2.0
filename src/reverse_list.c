#include "../headers/student_management_system.h"
#include <stdio.h>

void reverse_list(student **student_head) {

    if (*student_head == NULL || (*student_head)->next == NULL) {
        printf("\nThere are not enough students in the list to reverse it.\n\n");
        return;
    }

    student *previous = NULL;
    student *current = *student_head;
    student *next = NULL;

    while (current != NULL) {
        next = current->next;
        current->next = previous;
        previous = current;
        current = next;
    }

    *student_head = previous;

    if (current_direction == DIR_NORMAL) {
        current_direction = DIR_REVERSED;
    } else {
        current_direction = DIR_NORMAL;
    }

    const char *direction_mode[] = {"Normal", "Reversed"};

    printf("\nList successfully reversed! System direction updated (Current "
           "direction: %s).\n\n",
           direction_mode[current_direction]);
}