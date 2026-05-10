#include "../headers/student_management_system.h"
#include <stdio.h>
#include <string.h>

/*
 * Sorts the student list based on the current sorting mode and direction. Uses a data-swapping bubble sort algorithm 
 * to maintain pointer integrity.
 */
void sort_student_list(student **student_head) {

    if (*student_head == NULL) {
        printf("\n!ERROR! Student list already empty\n");
        return;
    } else if ((*student_head)->next == NULL) {
        printf("\n!ERROR! Student list have only 1 element. No sorting needed");
        return;
    }

    int swap_flag;
    student *current_student;

    do {

        swap_flag = 0;
        current_student = *student_head;

        while (current_student->next != NULL) {

            if (compare_students(current_student->next, current_student)) {
                swap_student(current_student, current_student->next);
                swap_flag = 1;
            }

            current_student = current_student->next;
        }

    } while (swap_flag);

    const char *sort_list_mode[] = {"ID", "GPA", "Name"};
    const char *direction_mode[] = {"Normal", "Reversed"};

    printf("\nList has been successfully sorted by %s (%s order)\n\n", sort_list_mode[current_mode],
           direction_mode[current_direction]);

    return;
}