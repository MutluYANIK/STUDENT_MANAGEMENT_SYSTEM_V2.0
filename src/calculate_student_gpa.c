#include "../headers/safe_input.h"
#include "../headers/student_management_system.h"
#include <stdio.h>
#include <string.h>

void calculate_student_gpa(student *target_student) {

    float total_weighted_score = 0.0;
    int total_credits = 0;
    
    enrollment *current_enrollment = target_student->records;

    while(current_enrollment != NULL) {

        if(current_enrollment->course_average == -1 || strcmp(current_enrollment->letter_grade, "--") == 0) {
            current_enrollment = current_enrollment->next;
            continue;
        }

        float letter_grade_value = get_letter_grade_value(current_enrollment->letter_grade);
        int current_credit = current_enrollment->lecture->lecture_credit;

        total_weighted_score += letter_grade_value * current_credit;
        total_credits += current_credit;

        current_enrollment = current_enrollment->next;
    }

    if(total_credits == 0) {
        target_student->GPA = 0.0;
    } else {
        target_student->GPA = total_weighted_score / total_credits;
    }
}