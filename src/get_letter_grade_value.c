#include "../headers/student_management_system.h"
#include <stdio.h>
#include <string.h>

float get_letter_grade_value(const char *letter_grade) {
    
    if (strcmp(letter_grade, "AA") == 0) {
        return 4.0;
    } else if (strcmp(letter_grade, "BA") == 0) {
        return 3.5;
    } else if (strcmp(letter_grade, "BB") == 0) {
        return 3.0;
    } else if (strcmp(letter_grade, "CB") == 0) {
        return 2.5;
    } else if (strcmp(letter_grade, "CC") == 0) {
        return 2.0;
    } else if (strcmp(letter_grade, "DC") == 0) {
        return 1.5;
    } else if (strcmp(letter_grade, "DD") == 0) {
        return 1.0;
    } else if (strcmp(letter_grade, "FD") == 0) {
        return 0.5;
    } else {
        return 0.0;
    }
}