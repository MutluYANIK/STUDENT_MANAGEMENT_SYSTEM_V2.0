#include "../headers/safe_input.h"
#include "../headers/student_management_system.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void delete_course(student *student_head, lecture **lecture_head) {

    // Check if the lecture list is empty returning an error message and return to the main menu.
    if (*lecture_head == NULL) {
        printf("!ERROR! Course list is already empty. Returning to the main menu...\n\n");
        return;
    }

    int loop_flag = 1;

    while (loop_flag) {

        int choose = get_safe_int_between(0, 2, 3,
                                          "\n[0] EXIT"
                                          "\n[1] DELETE A COURSE"
                                          "\n[2] CLEAR THE COURSE LIST\n"
                                          "\nEnter the operation you want to perform: ");

        switch (choose) {

        case 0:
            printf("\nOperation cancelled. Returning to the main menu...\n\n");
            loop_flag = 0;
            break;

        /*
         * Gives the user a chance to cancel the operation while taking the target course ID. Converts the input
         * target course ID to uppercase for consistency in comparison. Searches for the target course in the list. If
         * it is not in the list, prints an error message and returns previous menu. Takes confirmation from the user
         * before deleting the target course. Deletes the target course from the enrollment records for all students
         * and the course list. Updates the GPA for all enrolled students.
         */
        case 1: {

            if (*lecture_head == NULL) {
                printf("!ERROR! Course list is already empty. Returning to the previous menu...\n\n");
                return;
            }

            print_all_courses(*lecture_head);

            char temp_course_id[10];
            get_safe_string(3, temp_course_id, sizeof(temp_course_id),
                            "\nEnter the course ID you want to delete (or type 'exit' to cancel)");

            if (strcmp(temp_course_id, "exit") == 0) {
                printf("\nOperation cancelled. Returning to previous menu...\n\n");
                break;
            }

            for (int i = 0; temp_course_id[i] != '\0'; i++) {
                temp_course_id[i] = toupper((unsigned char)temp_course_id[i]);
            }

            lecture *selected_course = *lecture_head;
            lecture *previous_course = NULL;

            while (selected_course != NULL) {

                if (strcmp(selected_course->lecture_id, temp_course_id) == 0) {
                    break;
                }

                previous_course = selected_course;
                selected_course = selected_course->next;
            }

            if (selected_course == NULL) {
                printf("\n!ERROR! There is no course with ID '%s' in the course list\n\n", temp_course_id);
                break;
            }

            char prompt_message[100];

            snprintf(prompt_message, sizeof(prompt_message),
                     "\nYou are about to delete the course with ID '%s'\n"
                     "Enter 1 to continue (or 0 to cancel): ",
                     temp_course_id);

            int confirm = get_safe_int_between(0, 1, 3, prompt_message);

            if (confirm == 0) {
                printf("\nOperation cancelled. Returning to the previous menu...\n\n");
                break;
            }

            if (previous_course == NULL) {

                *lecture_head = selected_course->next;
                selected_course->next = NULL;

            } else {

                previous_course->next = selected_course->next;
                selected_course->next = NULL;
            }

            student *current_student = student_head;

            while (current_student != NULL) {

                enrollment *current_enrollment = current_student->records;
                enrollment *previous_enrollment = NULL;

                while (current_enrollment != NULL) {

                    if (strcmp(current_enrollment->lecture->lecture_id, temp_course_id) == 0) {

                        free(current_enrollment->scores);

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

            exam_template *current_exam = selected_course->exams;
            exam_template *previous_exam = NULL;

            while (current_exam != NULL) {

                previous_exam = current_exam;
                current_exam = current_exam->next;
                free(previous_exam);
            }

            free(selected_course);
            printf("\nCourse '%s' successfully deleted from the list\n"
                   "Students' GPA have been updated\n\n",
                   temp_course_id);
            break;
        }
        case 2: {

            if (*lecture_head == NULL) {
                printf("!ERROR! course list is already empty. Returning to the previous menu...\n\n");
                break;
            }

            int confirm = get_safe_int_between(0, 1, 3,
                                               "\nYou are about to clear the entire course list\n"
                                               "Enter 1 to continue (or 0 to cancel): ");

            if (confirm == 0) {
                printf("\nOperation cancelled. Returning to the previous menu...\n\n");
                break;
            }

            clear_the_course_list(student_head, lecture_head);
            loop_flag = 0;

            break;
        }
        }
    }
}