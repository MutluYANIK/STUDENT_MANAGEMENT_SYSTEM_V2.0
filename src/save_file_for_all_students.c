#include "../headers/safe_input.h"
#include "../headers/student_management_system.h"
#include <stdio.h>
#include <stdlib.h>

void save_file_for_all_students(student *student_head, course *course_head) {

    if (course_head == NULL) {
        printf("\n!ERROR! The course list is empty\n\n");
        return;
    }

    int loop_flag = 1;

    while (loop_flag) {

        int choice = get_safe_int_between(0, 4, 3,
                                          "\n[0] EXIT"
                                          "\n[2] GENERAL SUMMARY TABLE"
                                          "\n[3] FULL TRANSCRIPT"
                                          "\n[4] ONLY PASSED COURSES"
                                          "\n[5] ONLY FAILED COURSES"
                                          "\n[6] ONLY PENDING COURSES\n"
                                          "\nEnter the operation you want to perform: ");

        switch (choice) {
        case 0:
            printf("\nOperation cancelled. Returning to the previous menu...\n\n");
            loop_flag = 0;
            break;

        // Exports a condensed list of all students with their IDs, names, and total GPAs.
        case 1: {

            char file_name[100];

            get_safe_name(3, file_name, sizeof(file_name),
                          "\nEnter the name of the file you want to create (or type 'exit' to cancel): ");

            if (strcmp(file_name, "exit") == 0) {
                printf("\nOperation cancelled. Returning to the previous menu...\n\n");
                break;
            }

            strcat(file_name, ".txt");

            FILE *new_file = fopen(file_name, "w");

            if (new_file == NULL) {
                printf("\nFile couldn't be created\n\n");
                break;
            }

            fprintf(new_file,
                    "==========================================================================================="
                    "=============================\n");

            fprintf(new_file, "GENERAL SUMMARY TABLE\n");

            fprintf(new_file,
                    "==========================================================================================="
                    "=============================\n\n\n");

            fprintf(new_file, "%-25s%-50s%-8s\n", "STUDENT ID", "STUDENT NAME", "GPA");

            fprintf(new_file,
                    "-------------------------------------------------------------------------------------------"
                    "-----------------------------\n");

            student *current_student = student_head;

            while (current_student != NULL) {

                fprintf(new_file, "%-25u%-50.50s", current_student->id, current_student->name);

                if (current_student->GPA == -1.0) {

                    fprintf(new_file, "N/A");

                } else {

                    fprintf(new_file, "%.2f", current_student->GPA);
                }

                fprintf(new_file, "\n");
                current_student = current_student->next;
            }

            printf("\nGeneral summary table successfully saved to '%s'", file_name);

            fclose(new_file);
            break;
        }

        // Exports the complete academic history for every student in the system.
        case 2: {

            char file_name[100];

            get_safe_name(3, file_name, sizeof(file_name),
                          "\nEnter the name of the file you want to create (or type 'exit' to cancel): ");

            if (strcmp(file_name, "exit") == 0) {
                printf("\nOperation cancelled. Returning to the previous menu...\n\n");
                break;
            }

            strcat(file_name, ".txt");

            FILE *new_file = fopen(file_name, "w");

            if (new_file == NULL) {
                printf("\nFile couldn't be created\n\n");
                break;
            }

            student *current_student = student_head;

            int valid_students = 0;

            while (current_student != NULL) {

                if (current_student->records == NULL) {
                    current_student = current_student->next;
                    continue;
                }

                valid_students = 1;

                fprintf(new_file,
                        "==========================================================================================="
                        "=============================\n");

                fprintf(new_file,
                        "STUDENT TRANSCRIPT     |     ID: %u     |     NAME: %s     |     GPA: ", current_student->id,
                        current_student->name);

                if (current_student->GPA == -1.0) {

                    fprintf(new_file, "N/A");

                } else {

                    fprintf(new_file, "%.2f", current_student->GPA);
                }

                fprintf(new_file,
                        "\n==========================================================================================="
                        "=============================\n\n\n");

                fprintf(new_file, "%-14s%-25s%-10s%-49s%-8s%-8s%-8s\n", "COURSE ID", "COURSE NAME", "CREDIT",
                        "          EXAM GRADES", "AVG", "GRADE", "STATUS");

                fprintf(new_file,
                        "-------------------------------------------------------------------------------------------"
                        "-----------------------------\n");

                enrollment *current_enrollment = current_student->records;

                while (current_enrollment != NULL) {

                    char exam_buffer[75] = "";
                    char temp_buffer[75] = "";

                    fprintf(new_file, "%-14.14s", current_enrollment->course->course_id);
                    fprintf(new_file, "%-25.25s", current_enrollment->course->course_name);
                    fprintf(new_file, "%-10d", current_enrollment->course->course_credit);

                    exam_template *current_exam = current_enrollment->course->exams;
                    int exam_index = 0;

                    // Safely format exam scores to prevent buffer overflow in the table column
                    while (current_exam != NULL) {

                        int score = current_enrollment->scores[exam_index];

                        if (score == -1) {
                            snprintf(temp_buffer, sizeof(temp_buffer), "%s: N/A  ", current_exam->exam_name);

                        } else {
                            snprintf(temp_buffer, sizeof(temp_buffer), "%s: %d  ", current_exam->exam_name, score);
                        }

                        if (strlen(exam_buffer) > 50) {
                            break;
                        }

                        strcat(exam_buffer, temp_buffer);

                        current_exam = current_exam->next;
                        exam_index++;
                    }

                    fprintf(new_file, "%-49.49s", exam_buffer);

                    if (current_enrollment->course_average == -1) {
                        fprintf(new_file, "%-8.8s", "N/A");
                    } else {
                        fprintf(new_file, "%-8.2f", current_enrollment->course_average);
                    }

                    if (strcmp(current_enrollment->letter_grade, "--") == 0) {
                        fprintf(new_file, "%-8.8s", "N/A");
                    } else {
                        fprintf(new_file, "%-8.8s", current_enrollment->letter_grade);
                    }

                    if (strcmp(current_enrollment->letter_grade, "FF") == 0) {
                        fprintf(new_file, "%-8.8s\n", "FAILED");
                    } else if (strcmp(current_enrollment->letter_grade, "--") == 0) {
                        fprintf(new_file, "%-8.8s\n", "PENDING");
                    } else {
                        fprintf(new_file, "%-8.8s\n", "PASSED");
                    }

                    fprintf(new_file, "\n\n");

                    current_enrollment = current_enrollment->next;
                }

                current_student = current_student->next;
            }

            if (valid_students == 0) {
                fprintf(new_file, "NO ENROLLED STUDENTS FOUND IN THE SYSTEM");
            }

            printf("\nTranscripts successfully saved to '%s'\n\n", file_name);
            fclose(new_file);

            break;
        }

        // Iterates through all students and exports only the courses they have successfully passed.
        case 3: {

            char file_name[100];

            get_safe_name(3, file_name, sizeof(file_name),
                          "\nEnter the name of the file you want to create (or type 'exit' to cancel): ");

            if (strcmp(file_name, "exit") == 0) {
                printf("\nOperation cancelled. Returning to the previous menu...\n\n");
                break;
            }

            strcat(file_name, ".txt");

            FILE *new_file = fopen(file_name, "w");

            if (new_file == NULL) {
                printf("\nFile couldn't be created\n\n");
                break;
            }

            student *current_student = student_head;

            int valid_students = 0;

            while (current_student != NULL) {

                if (current_student->records == NULL) {
                    current_student = current_student->next;
                    continue;
                }

                int header_printed = 0;

                enrollment *current_enrollment = current_student->records;

                while (current_enrollment != NULL) {

                    if (strcmp(current_enrollment->letter_grade, "--") != 0 &&
                        strcmp(current_enrollment->letter_grade, "FF") != 0) {

                        valid_students = 1;

                        if (!header_printed) {

                            fprintf(new_file, "========================================================================"
                                              "================================================\n");

                            fprintf(new_file, "STUDENT TRANSCRIPT     |     ID: %u     |     NAME: %s     |     GPA: ",
                                    current_student->id, current_student->name);

                            if (current_student->GPA == -1.0) {

                                fprintf(new_file, "N/A");

                            } else {

                                fprintf(new_file, "%.2f", current_student->GPA);
                            }

                            fprintf(new_file, "\n======================================================================"
                                              "==================================================\n\n\n");

                            fprintf(new_file, "%-14s%-25s%-10s%-49s%-8s%-8s%-8s\n", "COURSE ID", "COURSE NAME",
                                    "CREDIT", "          EXAM GRADES", "AVG", "GRADE", "STATUS");

                            fprintf(new_file, "------------------------------------------------------------------------"
                                              "------------------------------------------------\n");
                        }

                        char exam_buffer[75] = "";
                        char temp_buffer[75] = "";

                        fprintf(new_file, "%-14.14s", current_enrollment->course->course_id);
                        fprintf(new_file, "%-25.25s", current_enrollment->course->course_name);
                        fprintf(new_file, "%-10d", current_enrollment->course->course_credit);

                        exam_template *current_exam = current_enrollment->course->exams;
                        int exam_index = 0;

                        // Safely format exam scores to prevent buffer overflow in the table column
                        while (current_exam != NULL) {

                            int score = current_enrollment->scores[exam_index];

                            snprintf(temp_buffer, sizeof(temp_buffer), "%s: %d  ", current_exam->exam_name, score);

                            if (strlen(exam_buffer) > 50) {
                                break;
                            }

                            strcat(exam_buffer, temp_buffer);

                            current_exam = current_exam->next;
                            exam_index++;
                        }

                        fprintf(new_file, "%-49.49s", exam_buffer);
                        fprintf(new_file, "%-8.2f", current_enrollment->course_average);
                        fprintf(new_file, "%-8.8s", current_enrollment->letter_grade);
                        fprintf(new_file, "%-8.8s\n", "PASSED");
                    }

                    fprintf(new_file, "\n\n");

                    current_enrollment = current_enrollment->next;
                }

                current_student = current_student->next;
            }

            if (valid_students == 0) {
                fprintf(new_file, "NO ENROLLED STUDENTS FOUND IN THE SYSTEM");
            }

            printf("\nTranscripts successfully saved to '%s'\n\n", file_name);
            fclose(new_file);

            break;
        }

        // Iterates through all students and exports only the courses with an 'FF' grade.
        case 4: {

            char file_name[100];

            get_safe_name(3, file_name, sizeof(file_name),
                          "\nEnter the name of the file you want to create (or type 'exit' to cancel): ");

            if (strcmp(file_name, "exit") == 0) {
                printf("\nOperation cancelled. Returning to the previous menu...\n\n");
                break;
            }

            strcat(file_name, ".txt");

            FILE *new_file = fopen(file_name, "w");

            if (new_file == NULL) {
                printf("\nFile couldn't be created\n\n");
                break;
            }

            student *current_student = student_head;

            int valid_students = 0;

            while (current_student != NULL) {

                if (current_student->records == NULL) {
                    current_student = current_student->next;
                    continue;
                }

                int header_printed = 0;

                enrollment *current_enrollment = current_student->records;

                while (current_enrollment != NULL) {

                    if (strcmp(current_enrollment->letter_grade, "FF") == 0) {

                        valid_students = 1;

                        if (!header_printed) {

                            fprintf(new_file, "========================================================================"
                                              "================================================\n");

                            fprintf(new_file, "STUDENT TRANSCRIPT     |     ID: %u     |     NAME: %s     |     GPA: ",
                                    current_student->id, current_student->name);

                            if (current_student->GPA == -1.0) {

                                fprintf(new_file, "N/A");

                            } else {

                                fprintf(new_file, "%.2f", current_student->GPA);
                            }

                            fprintf(new_file, "\n======================================================================"
                                              "==================================================\n\n\n");

                            fprintf(new_file, "%-14s%-25s%-10s%-49s%-8s%-8s%-8s\n", "COURSE ID", "COURSE NAME",
                                    "CREDIT", "          EXAM GRADES", "AVG", "GRADE", "STATUS");

                            fprintf(new_file, "------------------------------------------------------------------------"
                                              "------------------------------------------------\n");
                        }

                        char exam_buffer[75] = "";
                        char temp_buffer[75] = "";

                        fprintf(new_file, "%-14.14s", current_enrollment->course->course_id);
                        fprintf(new_file, "%-25.25s", current_enrollment->course->course_name);
                        fprintf(new_file, "%-10d", current_enrollment->course->course_credit);

                        exam_template *current_exam = current_enrollment->course->exams;
                        int exam_index = 0;

                        // Safely format exam scores to prevent buffer overflow in the table column
                        while (current_exam != NULL) {

                            int score = current_enrollment->scores[exam_index];

                            snprintf(temp_buffer, sizeof(temp_buffer), "%s: %d  ", current_exam->exam_name, score);

                            if (strlen(exam_buffer) > 50) {
                                break;
                            }

                            strcat(exam_buffer, temp_buffer);

                            current_exam = current_exam->next;
                            exam_index++;
                        }

                        fprintf(new_file, "%-49.49s", exam_buffer);
                        fprintf(new_file, "%-8.2f", current_enrollment->course_average);
                        fprintf(new_file, "%-8.8s", current_enrollment->letter_grade);
                        fprintf(new_file, "%-8.8s\n", "FAILED");
                    }

                    fprintf(new_file, "\n\n");

                    current_enrollment = current_enrollment->next;
                }

                current_student = current_student->next;
            }

            if (valid_students == 0) {
                fprintf(new_file, "NO ENROLLED STUDENTS FOUND IN THE SYSTEM");
            }

            printf("\nTranscripts successfully saved to '%s'\n\n", file_name);
            fclose(new_file);

            break;
        }

        // Iterates through all students and exports courses currently lacking a finalized grade ('--').
        case 5: {

            char file_name[100];

            get_safe_name(3, file_name, sizeof(file_name),
                          "\nEnter the name of the file you want to create (or type 'exit' to cancel): ");

            if (strcmp(file_name, "exit") == 0) {
                printf("\nOperation cancelled. Returning to the previous menu...\n\n");
                break;
            }

            strcat(file_name, ".txt");

            FILE *new_file = fopen(file_name, "w");

            if (new_file == NULL) {
                printf("\nFile couldn't be created\n\n");
                break;
            }

            student *current_student = student_head;

            int valid_students = 0;

            while (current_student != NULL) {

                if (current_student->records == NULL) {
                    current_student = current_student->next;
                    continue;
                }

                int header_printed = 0;

                enrollment *current_enrollment = current_student->records;

                while (current_enrollment != NULL) {

                    if (strcmp(current_enrollment->letter_grade, "--") == 0) {

                        valid_students = 1;

                        if (!header_printed) {

                            fprintf(new_file, "========================================================================"
                                              "================================================\n");

                            fprintf(new_file, "STUDENT TRANSCRIPT     |     ID: %u     |     NAME: %s     |     GPA: ",
                                    current_student->id, current_student->name);

                            if (current_student->GPA == -1.0) {

                                fprintf(new_file, "N/A");

                            } else {

                                fprintf(new_file, "%.2f", current_student->GPA);
                            }

                            fprintf(new_file, "\n======================================================================"
                                              "==================================================\n\n\n");

                            fprintf(new_file, "%-14s%-25s%-10s%-49s%-8s%-8s%-8s\n", "COURSE ID", "COURSE NAME",
                                    "CREDIT", "          EXAM GRADES", "AVG", "GRADE", "STATUS");

                            fprintf(new_file, "------------------------------------------------------------------------"
                                              "------------------------------------------------\n");
                        }

                        char exam_buffer[75] = "";
                        char temp_buffer[75] = "";

                        fprintf(new_file, "%-14.14s", current_enrollment->course->course_id);
                        fprintf(new_file, "%-25.25s", current_enrollment->course->course_name);
                        fprintf(new_file, "%-10d", current_enrollment->course->course_credit);

                        exam_template *current_exam = current_enrollment->course->exams;
                        int exam_index = 0;

                        // Safely format exam scores to prevent buffer overflow in the table column
                        while (current_exam != NULL) {

                            int score = current_enrollment->scores[exam_index];

                            if (score == -1) {
                                snprintf(temp_buffer, sizeof(temp_buffer), "%s: N/A  ", current_exam->exam_name);

                            } else {
                                snprintf(temp_buffer, sizeof(temp_buffer), "%s: %d  ", current_exam->exam_name, score);
                            }

                            if (strlen(exam_buffer) > 50) {
                                break;
                            }

                            strcat(exam_buffer, temp_buffer);

                            current_exam = current_exam->next;
                            exam_index++;
                        }

                        fprintf(new_file, "%-49.49s", exam_buffer);
                        fprintf(new_file, "%-8.8s", "N/A");
                        fprintf(new_file, "%-8.8s", "N/A");
                        fprintf(new_file, "%-8.8s\n", "PENDING");
                    }

                    fprintf(new_file, "\n\n");

                    current_enrollment = current_enrollment->next;
                }

                current_student = current_student->next;
            }

            if (valid_students == 0) {
                fprintf(new_file, "NO ENROLLED STUDENTS FOUND IN THE SYSTEM");
            }

            printf("\nTranscripts successfully saved to '%s'\n\n", file_name);
            fclose(new_file);

            break;
        }
        }
    }
}