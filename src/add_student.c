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
    unsigned int temp_student_id = get_safe_unsigned_int(3, "Enter student ID (or 0 to return to main menu): ");

    if (temp_student_id == 0) {
        printf("\nOperation cancelled. Returning to menu...\n\n");
        return 0;
    }

    if (id_check(*student_head, temp_student_id) == 1) {
        printf("\n!ERROR! A student with ID %u already exists in the list!\n\n", temp_student_id);
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

    new_student->id = temp_student_id;
    strcpy(new_student->name, temp_student_name);

    new_student->records = NULL;
    new_student->next = NULL;

    lecture *current_lecture = lecture_head;

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

        /*
         * Checks if the memory allocation for the new enrollment was successful. If it fails, it prints an error
         * message, frees any previously allocated memory for the new student and their enrollments to prevent memory
         * leaks, and returns 0 to indicate that the operation was unsuccessful.
         */
        enrollment *new_enrollment = (enrollment *)malloc(sizeof(enrollment));

        if (new_enrollment == NULL) {
            printf("\n!ERROR! Memory allocation failed!\n\n");

            while (new_student->records != NULL) {
                enrollment *temp_enrollment = new_student->records;
                new_student->records = new_student->records->next;
                free(temp_enrollment->scores);
                free(temp_enrollment);
            }
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

    /*
     * It takes the exam scores for each enrolled lecture, calculates the course average and letter grade
     * for each enrollment
     */
    enrollment *current_enrollment = new_student->records;

    while (current_enrollment != NULL) {

        printf("\nEntering grades for lecture '%s'\n", current_enrollment->lecture->lecture_name);

        exam_template *current_exam = current_enrollment->lecture->exams;
        int i = 0;

        while (current_exam != NULL) {

            char prompt_message[150];

            snprintf(prompt_message, sizeof(prompt_message),
                     "Enter grade for exam '%s' (or -1 for pending grade): ", current_exam->exam_name);

            int score = get_safe_int_between(3, -1, 100, prompt_message);

            current_enrollment->scores[i] = score;
            i++;
            current_exam = current_exam->next;
        }
        grade_calculator(current_enrollment);

        current_enrollment = current_enrollment->next;
    }

    calculate_student_gpa(new_student);

    // Inserts the new student into the linked list in sorted order based on the current sorting mode and sorting
    // direction.
    if (*student_head == NULL) {
        *student_head = new_student;
    } else if (compare_students(new_student, *student_head) == 1) {
        new_student->next = *student_head;
        *student_head = new_student;
    } else {
        student *current = *student_head;

        while (current->next != NULL && compare_students(new_student, current->next) == 0) {
            current = current->next;
        }
        new_student->next = current->next;
        current->next = new_student;
    }

    return 1;
}