#include "../headers/student_management_system.h"

void sort_student_list_by_id(student **student_head){

    current_mode = SORT_BY_ID;
    sort_student_list(student_head);
}