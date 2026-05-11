#include "../headers/safe_input.h"
#include "../headers/student_management_system.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void display_student(student *student_head, lecture *lecture_head) {

    if (student_head == NULL) {
        printf("\n!ERROR! The student list is empty\n\n");
        return;
    }

    if (lecture_head == NULL) {
        printf("\n!ERROR! The lecture list is empty\n\n");
        return;
    }

    int loop_flag = 1;

    while (loop_flag) {

        int choose = get_safe_int_between(0, 6, 3,
                                          "\n[0] EXIT"
                                          "\n[1] DISPLAY SPECIFIC STUDENT (BY ID)"
                                          "\n[2] DISPLAY ALL STUDENTS"
                                          "\n[3] FILTER STUDENTS BY LECTURE"
                                          "\n[4] SHOW CLASS AVERAGE FOR A LECTURE"
                                          "\n[5] SHOW OVERALL GPA"
                                          "\n[6] ADVANCED STUDENT SEARCH\n"
                                          "\nEnter the operation you want to perform: ");

        switch (choose) {
        case 0:
            printf("\nOperation cancelled. Returning to the main menu...\n\n");
            loop_flag = 1;
            break;
        
        // Gives the user a chance to cancel the operation while taking the student ID. Checks the student list
        case 1: {

            unsigned int temp_student_id =
                get_safe_unsigned_int(3, "\nEnter the student ID you want to display (or 0 to cancel): ");

            if (temp_student_id == 0) {
                printf("\nOperation cancelled. Returning to the previous menu...\n\n");
                break;
            }

            student *selected_student = student_head;

            while (selected_student != NULL) {

                if (selected_student->id == temp_student_id) {
                    break;
                }

                selected_student = selected_student->next;
            }

            if (selected_student == NULL) {
                printf("\nThere is no student in the list with ID '%u'\n\n", temp_student_id);
                break;
            }

            int mode = get_safe_int_between(1, 3, 3,
                                            "\n[1] SHOW ALL LECTURES"
                                            "\n[2] SHOW PASSED LECTURES"
                                            "\n[3] SHOW FAILED LECTURES\n"
                                            "\nEnter the operation you want to perform: ");
            
            int loop_flag_2 = 1;

            while(loop_flag_2){
            
                switch (mode) {

                case 1: {

                    printf("==============================================================================================="
                        "=========================\n");
                    printf("STUDENT TRANSCRIPT     |     ID: %u     |     NAME: %s     |     GPA: %.2f\n",
                        selected_student->id, selected_student->name, selected_student->GPA);
                    printf("==============================================================================================="
                        "=========================\n");
                    printf("%-14s %-25s %-10s %-40s  %-8s  %-8s  %-8s\n", "LECTURE ID", "LECTURE NAME", "CREDIT",
                        "          EXAM SCORES", "AVG", "GRADE", "STATUS");
                    printf("-----------------------------------------------------------------------------------------------"
                        "-------------------------\n");

                    enrollment *current_enrollment = selected_student->records;

                    while (current_enrollment != NULL) {
                        
                        char exam_buffer[75] = "";
                        char temp_buffer[75] = "";

                        printf("%-14.14s", current_enrollment->lecture->lecture_id);
                        printf("%-25.25s", current_enrollment->lecture->lecture_name);
                        printf("%-10d", current_enrollment->lecture->lecture_credit);

                        exam_template *current_exam = current_enrollment->lecture->exams;
                        int exam_index = 0;

                        while (current_exam != NULL) {

                            int score = current_enrollment->scores[exam_index];

                            if (score == -1) {
                                snprintf(temp_buffer, sizeof(temp_buffer), "%s: N/A  ", current_exam->exam_name);

                            } else {
                                snprintf(temp_buffer, sizeof(temp_buffer), "%s: %d  ", current_exam->exam_name, score);
                            }

                            if(strlen(exam_buffer) > 40){
                                break;
                            }

                            strcat(exam_buffer, temp_buffer);

                            current_exam = current_exam->next;
                            exam_index++;
                        }

                        printf("%-40.40s", exam_buffer);
                        
                        if(current_enrollment->course_average == -1){
                            printf("%-8.8s", "N/A");
                        } else {
                            printf("%-8.2f", current_enrollment->course_average);
                        }
                        
                        printf("%-8.8s", current_enrollment->letter_grade);

                        if (strcmp(current_enrollment->letter_grade, "FF") == 0) {
                            printf("%-8.8s\n", "FAILED");
                        } else if (strcmp(current_enrollment->letter_grade, "--") == 0) {
                            printf("%-8.8s\n", "PENDING");
                        } else {
                            printf("%-8.8s\n", "PASSED");
                        }

                        current_enrollment = current_enrollment->next;
                    }

                    break;
                }
                }
            }
        }
        }
    }
}