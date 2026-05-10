#include "../headers/student_management_system.h"


// Swaps the data payload of two student nodes without breaking their original positions in the linked list.
void swap_student(student *student_1, student *student_2){

    student *temp_1_next = student_1->next;
    student *temp_2_next = student_2->next;

    student temp = *student_1;

    *student_1 = *student_2;
    *student_2 = temp;

    student_1->next = temp_1_next;
    student_2->next = temp_2_next;

}