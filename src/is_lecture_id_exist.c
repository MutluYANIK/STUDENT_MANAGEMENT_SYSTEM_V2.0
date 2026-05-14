#include "../headers/student_management_system.h"
#include <string.h>
#include <stdio.h>

int is_course_id_exist(lecture *course_head, const char *course_id) {

    lecture *current_course = course_head;
    
    while (current_course != NULL) {
        if (strcmp(current_course->lecture_id, course_id) == 0) {
            return 1;
        }
        current_course = current_course->next;
    }

    return 0;
}