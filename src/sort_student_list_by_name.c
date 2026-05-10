#include "../headers/student_management_system.h"

void sort_student_list_by_name(student **student_head){

    current_mode = SORT_BY_NAME;
    sort_student_list(student_head);
}