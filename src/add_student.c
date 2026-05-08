#include "../headers/safe_input.h"
#include "../headers/student_management_system.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int add_student(student **student_head, lecture *lecture_head) {

    /*
     * Gives you the chance to exit the function while taking student ID input, checks for duplicate
     * student IDs to maintain data integrity and prevent confusion in the system.
     */
    unsigned int temp = get_safe_unsigned_int(3, "Enter student ID (or 0 to return to main menu): ");

    if (temp == 0) {
        printf("\nOperation cancelled. Returning to menu...\n\n");
        return 0;
    }

    if (id_check(*student_head, temp) == 1) {
        return 1;
    }

    // Takes input for the student name and converts it to uppercase for consistency.
    char temp_student_name[40];
    get_safe_name(3, temp_student_name, sizeof(temp_student_name), "\nEnter student name: ");

    for (int i = 0; temp_student_name[i] != '\0'; i++) {
        temp_student_name[i] = toupper((unsigned char)temp_student_name[i]);
    }

    student *new_student = (student *)malloc(sizeof(student));

    if (new_student == NULL) {
        printf("\n!ERROR! Memory allocation failed!\n\n");
        return 0;
    }

    new_student->id = temp;
    strcpy(new_student->name, temp_student_name);

    new_student->records = NULL;
    new_student->next = NULL;

    lecture *current_lecture = lecture_head;

    // Checks if there are any lectures available for enrollment before proceeding with the enrollment process.
    if (current_lecture == NULL) {
        printf("\nNo lectures available to enroll in. Please add lectures first.\n\n");
        free(new_student);
        return 0;
    }

    // Displays available lectures to the user for enrollment.
    printf("\nAvailable lectures:\n");
    printf("--------------------------\n");
    while (current_lecture != NULL) {
        printf("Lecture ID: %s, Lecture Name: %s\n", current_lecture->lecture_id, current_lecture->lecture_name);
        current_lecture = current_lecture->next;
    }

    /*
     * Gives you the chance to exit the enrollment process while taking lecture ID input, checks for valid lecture IDs
     * to ensure students are enrolled in existing lectures, and checks for duplicate enrollments to prevent students
     * from being enrolled in the same lecture multiple times, which could lead to data inconsistencies and confusion in
     * the system.
     */
    while (1) {

        char temp_lecture_id[10];
        get_safe_string(3, temp_lecture_id, sizeof(temp_lecture_id),
                        "\nEnter lecture ID to enroll in (or type 'exit' to finish enrollment): ");

        if (strcmp(temp_lecture_id, "exit") == 0) {
            printf("\nEnrollment process completed for student '%s' (ID: %u).\n\n", new_student->name, new_student->id);
            break;
        }

        for (int i = 0; temp_lecture_id[i] != '\0'; i++) {
            temp_lecture_id[i] = toupper((unsigned char)temp_lecture_id[i]);
        }

        lecture *selected_lecture = lecture_head;

        while (selected_lecture != NULL) {
            if (strcmp(selected_lecture->lecture_id, temp_lecture_id) == 0) {
                break;
            }
            selected_lecture = selected_lecture->next;
        }

        if (selected_lecture == NULL) {
            printf("\n!ERROR! Lecture ID '%s' not found. Please try again.\n\n", temp_lecture_id);
            continue;
        }

        if (enrollment_check(new_student, selected_lecture) == 1) {
            printf("\n!ERROR! Student '%s' (ID: %u) is already enrolled in lecture '%s'. Please choose a different "
                   "lecture.\n\n",
                   new_student->name, new_student->id, selected_lecture->lecture_name);
            continue;
        }

        enrollment *new_enrollment = (enrollment *)malloc(sizeof(enrollment));

        if (new_enrollment == NULL) {
            printf("\n!ERROR! Memory allocation failed!\n\n");
            free(new_student);
            return 0;
        }

        new_enrollment->lecture = selected_lecture;

        int exam_count = 0;
        exam_template *temp_exam = selected_lecture->exams;

        while (temp_exam != NULL) {
            exam_count++;
            temp_exam = temp_exam->next;
        }

        if (exam_count > 0) {
            new_enrollment->scores = (int *)malloc(exam_count * sizeof(int));

            for (int i = 0; i < exam_count; i++) {
                new_enrollment->scores[i] = -1; // Initialize scores to -1 to indicate they haven't been entered yet
            }
        } else {
            new_enrollment->scores = NULL; // No exams for this lecture
        }

        new_enrollment->course_average = 0.0;
        strcpy(new_enrollment->letter_grade, "--");

        new_enrollment->next = new_student->records;
        new_student->records = new_enrollment;

        printf("\nStudent '%s' (ID: %u) enrolled in lecture '%s' successfully!\n\n", new_student->name, new_student->id,
               selected_lecture->lecture_name);
    }

    return 1;
}