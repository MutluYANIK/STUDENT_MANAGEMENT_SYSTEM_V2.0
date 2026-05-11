#include "../headers/safe_input.h"
#include "../headers/student_management_system.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void delete_student(student **student_head, lecture *lecture_head) {

    // Check if the student list is empty returning an error message and return to the main menu.
    if (*student_head == NULL) {
        printf("!ERROR! Student list is already empty. Terminating...\n\n");
        return;
    }

    int loop_flag = 1;

    while (loop_flag) {

        int choose = get_safe_int_between(0, 2, 3,
                                          "\n[0] Exit\n"
                                          "[1] Delete a student\n"
                                          "[2] Clear the student list\n\n"
                                          "Enter the operation you want to perform: ");

        switch (choose) {

        case 0:
            printf("\nOperation cancelled. Returning to main menu...\n\n");
            loop_flag = 0;
            break;

        case 1: {

            if (*student_head == NULL) {
                printf("\n!ERROR! Student list is already empty. Terminating...\n\n");
                return;
            }

            unsigned int temp_student_id =
                get_safe_unsigned_int(3, "\nEnter the student ID you want to delete (or 0 to cancel): ");

            if (temp_student_id == 0) {
                printf("\nOperation cancelled. Returning to previous menu");
                break;
            }

            student *current_student = *student_head;
            student *previous_student = NULL;

            while (current_student != NULL) {

                if (current_student->id == temp_student_id) {
                    break;
                }

                previous_student = current_student;
                current_student = current_student->next;
            }

            if (current_student == NULL) {
                printf("\n!ERROR! There is no student with ID '%u' in the student list\n\n", temp_student_id);
                break;
            }

            char prompt_message[150];
            snprintf(prompt_message, sizeof(prompt_message),
                     "\nYou are about to delete the student with ID '%u'\n"
                     "Enter 1 to continue (or 0 to cancel): ",
                     temp_student_id);

            int confirm = get_safe_int_between(0, 1, 3, prompt_message);

            if(confirm == 0){
                printf("\nOperation cancelled. Returning to previous menu...\n\n");
                break;
            }

            if(previous_student == NULL){
               
                *student_head = current_student->next;
                current_student->next = NULL;
            
            } else {

                previous_student->next = current_student->next;
                current_student->next = NULL;

            }

            enrollment *current_enrollment = current_student->records;
            enrollment *previous_enrollment = NULL;

            while(current_enrollment != NULL){

                previous_enrollment = current_enrollment;
                current_enrollment = current_enrollment->next;

                free(previous_enrollment->scores);
                free(previous_enrollment);

            }

            free(current_student);
            printf("\nThe student with ID: '%u' successfully deleted from the student list\n\n", temp_student_id);

            break;
        }

        case 2:{

            if (*student_head == NULL) {
                printf("\n!ERROR! Student list is already empty. Terminating...\n\n");
                return;
            }

            int confirm = get_safe_int_between(0, 1, 3,
                                               "\nYou are about to clear the entire student list\n"
                                               "Enter 1 to continue (or 0 to cancel): ");

            if (confirm == 0) {
                printf("\nOperation cancelled. Returning to previous menu...\n\n");
                break;
            }

            student *current_student = *student_head;
            student *previous_student = NULL;

            while (current_student != NULL) {

                enrollment *current_enrollment = current_student->records;
                enrollment *previous_enrollment = NULL;

                while (current_enrollment != NULL) {

                    previous_enrollment = current_enrollment;
                    current_enrollment = current_enrollment->next;

                    free(previous_enrollment->scores);
                    free(previous_enrollment);
                }

                previous_student = current_student;
                current_student = current_student->next;

                free(previous_student);
            }

            *student_head = NULL;
            printf("\nStudent list successfully cleared\n\n");
            loop_flag = 0;

            break;
        }
        }

        if(loop_flag != 0){
            loop_flag = get_safe_int_between(0, 1, 3, "\nEnter 1 to delete another student (or 0 to exit): ");
        }
    }
}