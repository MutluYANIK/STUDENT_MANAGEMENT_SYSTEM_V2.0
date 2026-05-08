#include "../headers/student_management_system.h"
#include <string.h>

int compare_students(student *s1, student *s2) {

    int result;

    switch (current_mode) {

    case SORT_BY_ID:
        result = s1->id < s2->id;
        break;

    case SORT_BY_GRADE:

        if (s1->total_grade == s2->total_grade) {
            result = s1->id < s2->id;
        } else {
            result = s1->total_grade > s2->total_grade;
        }
        break;

    case SORT_BY_NAME:

        if (strcmp(s1->name, s2->name) == 0) {
            result = s1->id < s2->id;
        } else {
            result = strcmp(s1->name, s2->name) < 0;
        }
        break;

    default:
        result = s1->id < s2->id;
    }

    if (current_direction == DIR_REVERSED) {
        return !result;
    }

    return result;
}