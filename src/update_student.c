#include "../headers/safe_input.h"
#include "../headers/student_management_system.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int update_student(student **student_head, lecture *lecture_head) {

    if (*student_head == NULL) {
        printf("\n!ERROR! No student available to update\n\n");
        return 0;
    }

    unsigned int temp_student_id = get_safe_unsigned_int(3, "\nEnter the student ID you want to update"
                                                            "(or 0 to return to main menu): ");

    if (temp_student_id == 0) {
        printf("\nOperation cancelled. Returning to main menu...\n\n");
        return 0;
    }

    student *current_student = *student_head;

    while (current_student != NULL) {

        if (current_student->id == temp_student_id) {
            break;
        }

        current_student = current_student->next;
    }

    if (current_student == NULL) {
        printf("\nThere is no student in the list with id '%u'\n\n", temp_student_id);
        return 1;
    }

    int loop_flag = 1;

    while (loop_flag) {
        int choice = get_safe_int_between(0, 8, 3,
                                          "\n[0] Exit\n"
                                          "[1] Update Student ID\n"
                                          "[2] Update Student Name\n"
                                          "[3] Drop a Lecture\n"
                                          "[4] Drop All Lectures\n"
                                          "[5] Enroll In a Lecture\n"
                                          "[6] Update an Exam Grade\n"
                                          "[7] Update All Exam Grades\n"
                                          "[8] Reset All Exam Grades\n\n"
                                          "Enter the operation you want to perform: ");

        switch (choice) {

        case 0:
            printf("\nOperation cancelled. Returning to main menu...\n\n");
            loop_flag = 0;
            break;

        case 1: {

            unsigned int new_id = get_safe_unsigned_int(3, "\nEnter the new id (or 0 to cancel): ");

            if (new_id == 0) {
                printf("\nOperation cancelled. Returning to previous menu...\n\n");
                break;
            }

            if (new_id == temp_student_id) {
                printf("\nStudent already has this ID");
                break;
            }

            if (id_check(student_head, new_id) == 1) {
                printf("\nAnother student already has this ID");
                break;
            }

            current_student->id = new_id;
            printf("\nStudent ID successfully updated\n\n");

            break;
        }

        case 2: {

            char new_name[50];

            get_safe_name(3, new_name, sizeof(new_name), "\nEnter the new name (or type 'exit' to cancel): ");

            if (strcmp(new_name, "exit") == 0) {
                printf("\nOperation cancelled. Returning to previous menu...\n\n");
                break;
            }

            strcpy(current_student->name, new_name);
            printf("\nStudent name successfully updated\n\n");
            
            break;
        }

        case 3: {

            if (lecture_head == NULL) {
                printf("\n!ERROR! There is no lecture in the list to drop. Returning to previous menu...\n\n");
                break;
            }

            if (current_student->records == NULL) {
                printf("\n!ERROR! There is no enrolled lecture in the list to drop. Returning to previous menu...\n\n");
                break;
            }

            print_enrolled_lectures(current_student);

            char target_lecture_id[10];

            get_safe_string(3, target_lecture_id, sizeof(target_lecture_id),
                            "\nEnter the lecture ID you want to drop"
                            "(or type 'exit' to cancel): ");

            if (strcmp(target_lecture_id, "exit") == 0) {
                printf("\nOperation cancelled. Returning to previous menu...\n\n");
                break;
            }

            for (int i = 0; target_lecture_id[i] != '\0'; i++) {
                target_lecture_id[i] = toupper((unsigned char)target_lecture_id[i]);
            }

            enrollment *current_enrollment = current_student->records;
            enrollment *previous_enrollment = NULL;

            while (current_enrollment != NULL) {

                if (strcmp(current_enrollment->lecture->lecture_id, target_lecture_id) == 0) {
                    break;
                }

                previous_enrollment = current_enrollment;
                current_enrollment = current_enrollment->next;
            }

            if (current_enrollment == NULL) {
                printf("\n!ERROR! The student with ID '%u' is not enrolled in the lecture with ID '%s'\n\n",
                       current_student->id, target_lecture_id);
                break;
            }

            char prompt_message[150];
            snprintf(prompt_message, sizeof(prompt_message),
                     "\nYou are about to drop the lecture with ID '%s' for the student with ID '%u'"
                     "\nEnter 1 to continue (or 0 to cancel): ",
                     target_lecture_id, current_student->id);

            int confirm = get_safe_int_between(0, 1, 3, prompt_message);

            if (confirm == 0) {
                printf("\nOperation cancelled. Returning to previous menu...\n\n");
                break;
            }

            if (previous_enrollment == NULL) {

                current_student->records = current_enrollment->next;
                current_enrollment->next = NULL;

            } else {

                previous_enrollment->next = current_enrollment->next;
                current_enrollment->next = NULL;
            }

            free(current_enrollment->scores);
            free(current_enrollment);

            calculate_student_gpa(current_student);
            printf("\nLecture successfully dropped and GPA updated\n\n");

            break;
        }

        case 4: {

            if (lecture_head == NULL) {
                printf("\n!ERROR! There is no lecture in the list to drop. Returning to previous menu...\n\n");
                break;
            }

            char prompt_message[150];

            snprintf(prompt_message, sizeof(prompt_message),
                     "You are about to drop all lectures for the student with ID '%u'"
                     "\nEnter 1 to continue (or 0 to cancel): ",
                     temp_student_id);

            int confirm = get_safe_int_between(0, 1, 3, prompt_message);

            if (confirm == 0) {
                printf("\nOperation cancelled. Returning to previous menu...\n\n");
                break;
            }

            enrollment *current_enrollment = current_student->records;
            enrollment *previous_enrollment = NULL;

            while (current_enrollment != NULL) {

                free(current_enrollment->scores);
                previous_enrollment = current_enrollment;
                current_enrollment = current_enrollment->next;
                free(previous_enrollment);
            }

            current_student->records = NULL;

            calculate_student_gpa(current_student);
            printf("\nAll the lectures successfully dropped and GPA updated\n\n");
            break;
        }

        case 5: {

            if (lecture_head == NULL) {
                printf("\n!ERROR! There is no lecture in the list to enroll in. Returning to previous menu...\n\n");
                break;
            }

            print_not_enrolled_lectures(current_student, lecture_head);

            char temp_lecture_id[10];

            get_safe_string(3, temp_lecture_id, sizeof(temp_lecture_id),
                            "\nEnter the lecture ID you want to enroll in (or type 'exit' to cancel): ");

            if (strcmp(temp_lecture_id, "exit") == 0) {
                printf("\nOperation cancelled. Returning to previous menu...\n\n");
                break;
            }

            for (int i = 0; temp_lecture_id[i] != '\0'; i++) {
                temp_lecture_id[i] = toupper((unsigned char)temp_lecture_id[i]);
            }

            lecture *current_lecture = find_lecture(lecture_head, temp_lecture_id);

            if (current_lecture == NULL) {
                break;
            }

            enrollment *current_enrollment = current_student->records;

            int already_enrolled = 0;

            while (current_enrollment != NULL) {

                if (strcmp(current_lecture->lecture_id, current_enrollment->lecture->lecture_id) == 0) {
                    printf("\nThis student already enrolled in this lecture with ID '%s'", current_lecture->lecture_id);
                    already_enrolled = 1;
                    break;
                }

                current_enrollment = current_enrollment->next;
            }

            if (already_enrolled) {
                break;
            }

            enrollment *new_enrollment = (enrollment *)malloc(sizeof(enrollment));

            if (new_enrollment == NULL) {
                printf("\n!ERROR! Memory allocation failed \n\n");
                return 0;
            }

            new_enrollment->lecture = current_lecture;

            exam_template *current_exam = new_enrollment->lecture->exams;
            int exam_counter = 0;

            while (current_exam != NULL) {

                exam_counter++;
                current_exam = current_exam->next;
            }

            if (exam_counter > 0) {

                new_enrollment->scores = (int *)malloc(exam_counter * sizeof(int));

                for (int i = 0; i < exam_counter; i++) {
                    new_enrollment->scores[i] = -1;
                }
            } else {
                new_enrollment->scores = NULL;
            }

            new_enrollment->course_average = 0.0;
            strcpy(new_enrollment->letter_grade, "--");

            new_enrollment->next = current_student->records;
            current_student->records = new_enrollment;

            calculate_student_gpa(current_student);

            printf("\nStudent successfully enrolled in '%s'\n\n", temp_lecture_id);
            break;
        }

        case 6: {

            if (lecture_head == NULL) {
                printf("\n!ERROR! There is no lecture in the list to update an exam grade. Returning to previous "
                       "menu...\n\n");
                break;
            }

            if (current_student->records == NULL) {
                printf("\n!ERROR! There is no enrolled lecture in the list to update an exam grade. Returning to "
                       "previous menu...\n\n");
                break;
            }

            print_enrolled_lectures(current_student);

            char target_lecture_id[10];

            get_safe_string(3, target_lecture_id, sizeof(target_lecture_id),
                            "\nEnter the lecture id you want to update an exam grade (or type 'exit' to cancel)");

            if (strcmp(target_lecture_id, "exit") == 0) {
                printf("\nOperation cancelled. Returning to previous menu...\n\n");
                break;
            }

            for (int i = 0; target_lecture_id[i] != '\0'; i++) {
                target_lecture_id[i] = toupper((unsigned char)target_lecture_id[i]);
            }

            enrollment *current_enrollment = current_student->records;

            while (current_enrollment != NULL) {

                if (strcmp(current_enrollment->lecture->lecture_id, target_lecture_id) == 0) {
                    break;
                }

                current_enrollment = current_enrollment->next;
            }

            if (current_enrollment == NULL) {
                printf("\n!ERROR! The student is not enrolled in the lecture '%s'.\n\n", target_lecture_id);
                break;
            }

            exam_template *current_exam = current_enrollment->lecture->exams;
            printf("\nAvailable exams:\n");

            while (current_exam != NULL) {

                printf("%s\n", current_exam->exam_name);
                current_exam = current_exam->next;
            }

            char target_exam_name[20];

            get_safe_string(3, target_exam_name, sizeof(target_exam_name),
                            "\nEnter the exam name you want to update exam grade (or type 'exit' to cancel)");

            if (strcmp(target_exam_name, "exit") == 0) {
                printf("\nOperation cancelled. Returning to previous menu...\n\n");
                break;
            }

            for (int i = 0; target_exam_name[i] != '\0'; i++) {
                target_exam_name[i] = toupper((unsigned char)target_exam_name[i]);
            }

            current_exam = current_enrollment->lecture->exams;
            int exam_counter = 0;

            while (current_exam != NULL) {

                if (strcmp(current_exam->exam_name, target_exam_name) == 0) {
                    break;
                }

                exam_counter++;
                current_exam = current_exam->next;
            }

            if (current_exam == NULL) {
                printf("\n!ERROR! There is no exam in the list with the name '%s'\n\n", target_exam_name);
                break;
            }

            current_enrollment->scores[exam_counter] = get_safe_int_between(0, 100, 3, "\nEnter the new exam grade: ");

            grade_calculator(current_enrollment);
            calculate_student_gpa(current_student);

            printf("\nThe exam grade, course average and GPA successfully updated\n\n");
            break;
        }

        case 7: {

            if (lecture_head == NULL) {
                printf("\n!ERROR! There is no lecture in the list to update exam grades. Returning to previous "
                       "menu...\n\n");
                break;
            }

            if (current_student->records == NULL) {
                printf("\n!ERROR! There is no enrolled lecture in the list to update exam grades. Returning to "
                       "previous menu...\n\n");
                break;
            }

            int confirm = get_safe_int_between(0, 1, 3,
                                               "\nYou are about to update all exam grades"
                                               "\nEnter 1 to continue (or 0 to cancel): ");

            if (confirm == 0) {
                printf("\nOperation cancelled. Returning to previous menu...\n\n");
                break;
            }

            enrollment *current_enrollment = current_student->records;

            while (current_enrollment != NULL) {

                exam_template *current_exam = current_enrollment->lecture->exams;
                int exam_counter = 0;

                while (current_exam != NULL) {

                    char prompt_message[150];
                    snprintf(prompt_message, sizeof(prompt_message),
                             "\nEnter the new '%s' grade for lecture '%s': ", current_exam->exam_name,
                             current_enrollment->lecture->lecture_name);

                    current_enrollment->scores[exam_counter] = get_safe_int_between(0, 100, 3, prompt_message);

                    exam_counter++;

                    current_exam = current_exam->next;
                }

                grade_calculator(current_enrollment);

                current_enrollment = current_enrollment->next;
            }

            calculate_student_gpa(current_student);
            printf("\nAll exam grades, course averages and GPA successfully updated\n\n");

            break;
        }

        case 8: {

            if (lecture_head == NULL) {
                printf("\n!ERROR! There is no lecture in the list to reset exam grades. Returning to previous "
                       "menu...\n\n");
                break;
            }

            if (current_student->records == NULL) {
                printf("\n!ERROR! There is no enrolled lecture in the list to reset exam grades. Returning to "
                       "previous menu...\n\n");
                break;
            }

            int confirm = get_safe_int_between(0, 1, 3,
                                               "\nYou are about to reset all exam grades"
                                               "\nEnter 1 to continue (or 0 to cancel): ");

            if (confirm == 0) {
                printf("\nOperation cancelled. Returning to previous menu...\n\n");
                break;
            }

            enrollment *current_enrollment = current_student->records;

            while (current_enrollment != NULL) {

                exam_template *current_exam = current_enrollment->lecture->exams;
                int exam_counter = 0;

                while (current_exam != NULL) {

                    current_enrollment->scores[exam_counter] = -1;
                    exam_counter++;

                    current_exam = current_exam->next;
                }

                grade_calculator(current_enrollment);

                current_enrollment = current_enrollment->next;
            }

            calculate_student_gpa(current_student);
            printf("\nAll exam grades, course averages and GPA have been successfully reset\n\n");

            break;
        }
        }

        if (loop_flag != 0) {
            loop_flag = get_safe_int_between(0, 1, 3, "\nEnter 1 to change something else (or 0 to exit): ");
        }
    }

    return 1;
}