#include "../headers/safe_input.h"
#include "../headers/student_management_system.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void delete_lecture(student *student_head, lecture **lecture_head) {

    if (lecture_head == NULL) {
        printf("!ERROR! Lecture list is already empty. Terminating...\n\n");
        return;
    }

    int loop_flag = 1;

    while (loop_flag) {

        int choose = get_safe_int_between(0, 2, 3,
                                          "\n[0] Exit\n"
                                          "[1] Delete a lecture\n"
                                          "[2] Clear the lecture list\n\n"
                                          "Enter the operation you want to perform: ");

        switch (choose) {

        case 0:
            printf("\nOperation cancelled. Returning to main menu...\n\n");
            loop_flag = 0;
            break;

        /*
         * Gives the user a chance to cancel the operation while taking the target lecture ID. Converts the input
         * target lecture ID to uppercase for consistency in comparison. Searches for the target lecture in the list. If
         * it is not in the list, prints an error message and breaks the loop. Takes confirmation from the user
         * before deleting the target lecture. Deletes the target lecture from the enrollment records for all students
         * and the lecture list. Updates the GPA for all enrolled students.
         */
        case 1: {

            char temp_lecture_id[10];
            get_safe_string(3, temp_lecture_id, sizeof(temp_lecture_id),
                            "\n Enter the lecture ID you want to delete (or type 'exit' to cancel)");

            if (strcmp(temp_lecture_id, "exit") == 0) {
                printf("\nOperation cancelled. Returning to previous menu");
                break;
            }

            for (int i = 0; temp_lecture_id[i] != '\0'; i++) {
                temp_lecture_id[i] = toupper((unsigned char)temp_lecture_id[i]);
            }

            lecture *selected_lecture = *lecture_head;
            lecture *previous_lecture = NULL;

            while (selected_lecture != NULL) {

                if (strcmp(selected_lecture->lecture_id, temp_lecture_id) == 0) {
                    break;
                }

                previous_lecture = selected_lecture;
                selected_lecture = selected_lecture->next;
            }

            if (selected_lecture == NULL) {
                printf("\n!ERROR! There is no lecture with ID '%s' in the lecture list\n\n", temp_lecture_id);
                break;
            }

            char prompt_message[100];

            snprintf(prompt_message, sizeof(prompt_message),
                     "\nYou are going to delete the lecture with ID '%s'\n"
                     "Enter 1 to continue (or 0 to cancel): ",
                     temp_lecture_id);

            int confirm = get_safe_int_between(0, 1, 3, prompt_message);

            if (confirm == 0) {

                printf("\nOperation cancelled. Returning to previous menu...\n\n");
                break;
            }

            if (previous_lecture == NULL) {

                *lecture_head = selected_lecture->next;
                selected_lecture->next = NULL;

            } else {

                previous_lecture->next = selected_lecture->next;
                selected_lecture->next = NULL;
            }

            student *current_student = student_head;

            while (current_student != NULL) {

                enrollment *current_enrollment = current_student->records;
                enrollment *previous_enrollment = NULL;

                while (current_enrollment != NULL) {

                    if (strcmp(current_enrollment->lecture->lecture_id, temp_lecture_id) == 0) {

                        free(current_enrollment->scores);
                        current_enrollment->scores = NULL;

                        if (previous_enrollment == NULL) {

                            current_student->records = current_enrollment->next;

                        } else {

                            previous_enrollment->next = current_enrollment->next;
                        }

                        free(current_enrollment);
                        calculate_student_gpa(current_student);

                        break;
                    }

                    previous_enrollment = current_enrollment;
                    current_enrollment = current_enrollment->next;
                }

                current_student = current_student->next;
            }

            exam_template *current_exam = selected_lecture->exams;
            exam_template *previous_exam = NULL;

            while (current_exam != NULL) {

                previous_exam = current_exam;
                current_exam = current_exam->next;
                free(previous_exam);
            }

            free(selected_lecture);
            printf("\nLecture '%s' successfully deleted from the list\n"
                   "Students' GPA have been updated\n\n",
                   temp_lecture_id);
            break;
        }
        }
    }
}