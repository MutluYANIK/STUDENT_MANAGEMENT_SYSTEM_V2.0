#include "../headers/safe_input.h"
#include "../headers/student_management_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void grade_calculator(enrollment *enrollment_record) {

    if (enrollment_record == NULL || enrollment_record->lecture == NULL) {
        printf("\n!ERROR! Invalid enrollment record provided to grade_calculator.\n\n");
        return;
    }

    exam_template *current_exam = enrollment_record->lecture->exams;

    if (current_exam == NULL) {
        enrollment_record->course_average = -1; // No exams, set average to -1 to indicate N/A
        strcpy(enrollment_record->letter_grade, "--");
        return;
    }

    float total_score = 0.0;
    int exam_count = 0;
    
    while (current_exam != NULL) {
        if (enrollment_record->scores[exam_count] == -1) {
            enrollment_record->course_average = -1; // Pending grade, set average to -1 to indicate N/A
            strcpy(enrollment_record->letter_grade, "--");
            return;
        }
        total_score += enrollment_record->scores[exam_count] * (current_exam->exam_percentage / 100.0);
        exam_count++;
        current_exam = current_exam->next;
    }
    enrollment_record->course_average = total_score;

    if (enrollment_record->course_average >= 90) {
        strcpy(enrollment_record->letter_grade, "AA");
    } else if (enrollment_record->course_average >= 85) {
        strcpy(enrollment_record->letter_grade, "BA");
    } else if (enrollment_record->course_average >= 80) {
        strcpy(enrollment_record->letter_grade, "BB");
    } else if (enrollment_record->course_average >= 75) {
        strcpy(enrollment_record->letter_grade, "CB");
    } else if (enrollment_record->course_average >= 70) {
        strcpy(enrollment_record->letter_grade, "CC");
    } else if (enrollment_record->course_average >= 65) {
        strcpy(enrollment_record->letter_grade, "DC");
    } else if (enrollment_record->course_average >= 60) {
        strcpy(enrollment_record->letter_grade, "DD");
    } else if (enrollment_record->course_average >= 50) {
        strcpy(enrollment_record->letter_grade, "FD");
    } else {
        strcpy(enrollment_record->letter_grade, "FF");
    }
}