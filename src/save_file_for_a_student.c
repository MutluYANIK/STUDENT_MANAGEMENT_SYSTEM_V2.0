#include "../headers/safe_input.h"
#include "../headers/student_management_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Exports a specific student's transcript or filtered course list to a .txt file.
 * Features strict input validation and prevents file generation for empty/invalid records.
 */
void save_file_for_a_student(student *student_head, course *course_head) {

    // Check if system has any initialized courses or students
    if (course_head == NULL) {
        printf("\n!ERROR! The course list is empty\n\n");
        return;
    }

    // Prompt for the target student ID and locate them in the linked list
    unsigned int student_id = get_safe_unsigned_int(3, "\nEnter the student ID you want to save (or 0 to cancel): ");

    if (student_id == 0) {
        printf("\nOperation cancelled. Returning to the previous menu...\n\n");
        return;
    }

    student *current_student = student_head;

    while (current_student != NULL) {

        if (current_student->id == student_id) {
            break;
        }

        current_student = current_student->next;
    }

    if (current_student == NULL) {
        printf("\nThere is no student with ID '%u' in the student list", student_id);
        return;
    }

    if (current_student->records == NULL) {
        printf("\nThere is no enrolled courses for student with ID '%u'\n\n", student_id);
        return;
    }

    int loop_flag = 1;

    while (loop_flag) {

        int choice = get_safe_int_between(0, 4, 3,
                                          "\n[0] EXIT"
                                          "\n[1] FULL TRANSCRIPT"
                                          "\n[2] ONLY PASSED COURSES"
                                          "\n[3] ONLY FAILED COURSES"
                                          "\n[4] ONLY PENDING COURSES\n"
                                          "\nEnter the operation you want to perform: ");

        switch (choice) {

        case 0:
            printf("\nOperation cancelled. Returning to the previous menu...\n\n");
            loop_flag = 0;
            break;

        // Exports all enrolled courses regardless of their current status
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

                current_enrollment = current_enrollment->next;
            }

            printf("\nTranscripts successfully saved to '%s'\n\n", file_name);
            fclose(new_file);

            break;
        }

        // Filters and exports only the courses where the student has a passing grade
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

            enrollment *current_enrollment = current_student->records;

            int header_printed = 0;
            int passed_courses_exist = 0;

            while (current_enrollment != NULL) {

                if (strcmp(current_enrollment->letter_grade, "--") != 0 &&
                    strcmp(current_enrollment->letter_grade, "FF") != 0) {

                    // Print the file header only once if matching courses are found
                    if (!header_printed) {

                        fprintf(new_file, "============================================================================"
                                          "============================================\n");

                        fprintf(new_file, "STUDENT TRANSCRIPT     |     ID: %u     |     NAME: %s     |     GPA: ",
                                current_student->id, current_student->name);

                        if (current_student->GPA == -1.0) {

                            fprintf(new_file, "N/A");

                        } else {

                            fprintf(new_file, "%.2f", current_student->GPA);
                        }

                        fprintf(new_file, "\n=========================================================================="
                                          "==============================================\n\n\n");

                        header_printed = 1;
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

                    passed_courses_exist = 1;
                }

                current_enrollment = current_enrollment->next;
            }

            if (!passed_courses_exist) {

                fprintf(new_file, "No passed courses found for student with ID '%u'\n\n", student_id);

            } else {

                printf("\nPassed courses successfully saved to '%s'!\n", file_name);
            }

            fclose(new_file);

            break;
        }

        // Filters and exports only the courses with an 'FF' grade
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

            enrollment *current_enrollment = current_student->records;

            int header_printed = 0;
            int failed_courses_exist = 0;

            while (current_enrollment != NULL) {

                if (strcmp(current_enrollment->letter_grade, "FF") == 0) {

                    // Print the file header only once if matching courses are found
                    if (!header_printed) {

                        fprintf(new_file, "============================================================================"
                                          "============================================\n");

                        fprintf(new_file, "STUDENT TRANSCRIPT     |     ID: %u     |     NAME: %s     |     GPA: ",
                                current_student->id, current_student->name);

                        if (current_student->GPA == -1.0) {

                            fprintf(new_file, "N/A");

                        } else {

                            fprintf(new_file, "%.2f", current_student->GPA);
                        }

                        fprintf(new_file, "\n=========================================================================="
                                          "==============================================\n\n\n");

                        header_printed = 1;
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

                    failed_courses_exist = 1;
                }

                current_enrollment = current_enrollment->next;
            }

            if (!failed_courses_exist) {

                fprintf(new_file, "No failed courses found for student with ID '%u'\n\n", student_id);

            } else {

                printf("\nFailed courses successfully saved to '%s'\n\n", file_name);
            }

            fclose(new_file);

            break;
        }

        // Filters and exports courses where grades are not yet announced ('--')
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

            enrollment *current_enrollment = current_student->records;

            int header_printed = 0;
            int pending_courses_exist = 0;

            while (current_enrollment != NULL) {

                if (strcmp(current_enrollment->letter_grade, "--") == 0) {

                    // Print the file header only once if matching courses are found
                    if (!header_printed) {

                        fprintf(new_file, "============================================================================"
                                          "============================================\n");

                        fprintf(new_file, "STUDENT TRANSCRIPT     |     ID: %u     |     NAME: %s     |     GPA: ",
                                current_student->id, current_student->name);

                        if (current_student->GPA == -1.0) {

                            fprintf(new_file, "N/A");

                        } else {

                            fprintf(new_file, "%.2f", current_student->GPA);
                        }

                        fprintf(new_file, "\n=========================================================================="
                                          "==============================================\n\n\n");

                        header_printed = 1;
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


                    pending_courses_exist = 1;
                }

                current_enrollment = current_enrollment->next;
            }

            if (!pending_courses_exist) {

                fprintf(new_file, "No pending courses found for student with ID '%u'\n\n", student_id);

            } else {

                printf("\nPending courses successfully saved to '%s'\n\n", file_name);
            }

            fclose(new_file);

            break;
        }
        }
    }
}