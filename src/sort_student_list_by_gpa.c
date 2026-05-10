#include "../headers/student_management_system.h"

void sort_student_list_by_gpa(student **student_head){

    current_mode = SORT_BY_GPA;
    sort_student_list(student_head);
}