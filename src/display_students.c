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
        printf("\n!ERROR! The course list is empty\n\n");
        return;
    }

    int loop_flag = 1;

    while (loop_flag) {

        int choose = get_safe_int_between(0, 6, 3,
                                          "\n[0] EXIT"
                                          "\n[1] DISPLAY SPECIFIC STUDENT (BY ID)"
                                          "\n[2] DISPLAY ALL STUDENTS"
                                          "\n[3] FILTER STUDENTS BY COURSE"
                                          "\n[4] DISPLAY CLASS AVERAGE FOR A COURSE"
                                          "\n[5] DISPLAY OVERALL GPA"
                                          "\n[6] ADVANCED STUDENT SEARCH\n"
                                          "\nEnter the operation you want to perform: ");

        switch (choose) {
        case 0:
            printf("\nOperation cancelled. Returning to the main menu...\n\n");
            loop_flag = 1;
            break;
        /*
         * Gives the user a chance to cancel the operation while taking the student ID. Searches the ID in the student
         * list. If found, opens a sub-menu asking the use to choose a display filter (All, Passed or Failed courses).
         */
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

            int loop_flag_2 = 1;

            while (loop_flag_2) {

                int mode = get_safe_int_between(0, 3, 3,
                                                "\n[0] EXIT"
                                                "\n[1] DISPLAY ALL COURSES"
                                                "\n[2] DISPLAY PASSED COURSES"
                                                "\n[3] DISPLAY FAILED COURSES\n"
                                                "\nEnter the operation you want to perform: ");

                switch (mode) {

                case 0:
                    printf("\nOperation cancelled. Returning to the previous menu...\n\n");
                    loop_flag_2 = 0;
                    break;

                /*
                 * DISPLAY ALL COURSES: Iterates through the selected student's enrollment records and prints a
                 * formatted transcript table. Displays all courses' IDs, names, exam scores, course averages, grades
                 * and statuses
                 */
                case 1: {

                    printf("==========================================================================================="
                           "=============================\n");
                    printf("STUDENT TRANSCRIPT     |     ID: %u     |     NAME: %s     |     GPA: %.2f\n",
                           selected_student->id, selected_student->name, selected_student->GPA);
                    printf("==========================================================================================="
                           "=============================\n");
                    printf("%-14s %-25s %-10s %-40s  %-8s  %-8s  %-8s\n", "COURSE ID", "COURSE NAME", "CREDIT",
                           "          EXAM SCORES", "AVG", "GRADE", "STATUS");
                    printf("-------------------------------------------------------------------------------------------"
                           "-----------------------------\n");

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

                            if (strlen(exam_buffer) > 40) {
                                break;
                            }

                            strcat(exam_buffer, temp_buffer);

                            current_exam = current_exam->next;
                            exam_index++;
                        }

                        printf("%-40.40s", exam_buffer);

                        if (current_enrollment->course_average == -1) {
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

                /*
                 * DISPLAY PASSED COURSES: Uses a two-pass method to keep the CLI clean. Pass 1: Iterates through the
                 * records to check if the student has any passed courses. If there are none, prints a warning and
                 * breaks before printing the table headers.
                 */
                case 2: {

                    enrollment *current_enrollment = selected_student->records;
                    int passed_course_check = 0;

                    while (current_enrollment != NULL) {

                        if (strcmp(current_enrollment->letter_grade, "--") != 0 &&
                            strcmp(current_enrollment->letter_grade, "FF") != 0) {
                            passed_course_check = 1;
                        }

                        current_enrollment = current_enrollment->next;
                    }

                    if (!passed_course_check) {
                        printf("\nThere is no passed courses for student ID with '%u'\n\n", selected_student->id);
                        break;
                    }

                    printf("==========================================================================================="
                           "=============================\n");
                    printf("STUDENT TRANSCRIPT     |     ID: %u     |     NAME: %s     |     GPA: %.2f\n",
                           selected_student->id, selected_student->name, selected_student->GPA);
                    printf("==========================================================================================="
                           "=============================\n");
                    printf("%-14s %-25s %-10s %-40s  %-8s  %-8s  %-8s\n", "COURSE ID", "COURSE NAME", "CREDIT",
                           "          EXAM SCORES", "AVG", "GRADE", "STATUS");
                    printf("-------------------------------------------------------------------------------------------"
                           "-----------------------------\n");

                    current_enrollment = selected_student->records;

                    while (current_enrollment != NULL) {

                        while (current_enrollment != NULL && (strcmp(current_enrollment->letter_grade, "--") == 0 ||
                                                              strcmp(current_enrollment->letter_grade, "FF") == 0)) {
                            current_enrollment = current_enrollment->next;
                        }

                        if (current_enrollment == NULL) {
                            break;
                        }

                        char exam_buffer[75] = "";
                        char temp_buffer[75] = "";

                        printf("%-14.14s", current_enrollment->lecture->lecture_id);
                        printf("%-25.25s", current_enrollment->lecture->lecture_name);
                        printf("%-10d", current_enrollment->lecture->lecture_credit);

                        exam_template *current_exam = current_enrollment->lecture->exams;
                        int exam_index = 0;

                        while (current_exam != NULL) {

                            int score = current_enrollment->scores[exam_index];
                            snprintf(temp_buffer, sizeof(temp_buffer), "%s: %d  ", current_exam->exam_name, score);

                            if (strlen(exam_buffer) > 40) {
                                break;
                            }

                            strcat(exam_buffer, temp_buffer);

                            current_exam = current_exam->next;
                            exam_index++;
                        }

                        printf("%-40.40s", exam_buffer);
                        printf("%-8.2f", current_enrollment->course_average);
                        printf("%-8.8s", current_enrollment->letter_grade);
                        printf("%-8.8s\n", "PASSED");

                        current_enrollment = current_enrollment->next;
                    }

                    break;
                }
                /*
                 * DISPLAY FAILED COURSES: Uses the two-pass method for CLI safety. Pass 1: Iterates through the records
                 * to check if the student has any failed courses ("FF"). If there are none, prints a warning and breaks
                 * without drawing the table.
                 */
                case 3: {

                    enrollment *current_enrollment = selected_student->records;
                    int failed_course_check = 0;

                    while (current_enrollment != NULL) {

                        if (strcmp(current_enrollment->letter_grade, "FF") == 0) {
                            failed_course_check = 1;
                        }

                        current_enrollment = current_enrollment->next;
                    }

                    if (!failed_course_check) {
                        printf("\nThere is no failed courses for student ID with '%u'\n\n", selected_student->id);
                        break;
                    }

                    printf("==========================================================================================="
                           "=============================\n");
                    printf("STUDENT TRANSCRIPT     |     ID: %u     |     NAME: %s     |     GPA: %.2f\n",
                           selected_student->id, selected_student->name, selected_student->GPA);
                    printf("==========================================================================================="
                           "=============================\n");
                    printf("%-14s %-25s %-10s %-40s  %-8s  %-8s  %-8s\n", "COURSE ID", "COURSE NAME", "CREDIT",
                           "          EXAM SCORES", "AVG", "GRADE", "STATUS");
                    printf("-------------------------------------------------------------------------------------------"
                           "-----------------------------\n");

                    current_enrollment = selected_student->records;

                    while (current_enrollment != NULL) {

                        while (current_enrollment != NULL && (strcmp(current_enrollment->letter_grade, "--") == 0 ||
                                                              strcmp(current_enrollment->letter_grade, "FF") != 0)) {
                            current_enrollment = current_enrollment->next;
                        }

                        if (current_enrollment == NULL) {
                            break;
                        }

                        char exam_buffer[75] = "";
                        char temp_buffer[75] = "";

                        printf("%-14.14s", current_enrollment->lecture->lecture_id);
                        printf("%-25.25s", current_enrollment->lecture->lecture_name);
                        printf("%-10d", current_enrollment->lecture->lecture_credit);

                        exam_template *current_exam = current_enrollment->lecture->exams;
                        int exam_index = 0;

                        while (current_exam != NULL) {

                            int score = current_enrollment->scores[exam_index];
                            snprintf(temp_buffer, sizeof(temp_buffer), "%s: %d  ", current_exam->exam_name, score);

                            if (strlen(exam_buffer) > 40) {
                                break;
                            }

                            strcat(exam_buffer, temp_buffer);

                            current_exam = current_exam->next;
                            exam_index++;
                        }

                        printf("%-40.40s", exam_buffer);
                        printf("%-8.2f", current_enrollment->course_average);
                        printf("%-8.8s", current_enrollment->letter_grade);
                        printf("%-8.8s\n", "FAILED");

                        current_enrollment = current_enrollment->next;
                    }

                    break;
                }
                }
            }

            break;
        }

        case 2: {

            int loop_flag_2 = 1;

            while (loop_flag_2) {

                int mode = get_safe_int_between(0, 4, 3,
                                                "\n[0] EXIT"
                                                "\n[1] SHOW SUMMARY TABLE"
                                                "\n[2] SHOW FULL TRANSCRIPTS"
                                                "\n[3] SHOW ONLY PASSED COURSES"
                                                "\n[4] SHOW ONLY FAILED COURSES\n"
                                                "\nEnter the operation you want to perform: ");

                switch (mode) {

                case 0:
                    printf("\nOperation cancelled. Returning to the previous menu...\n\n");
                    loop_flag_2 = 0;
                    break;

                /*
                 * SUMMARY LIST: Iterates through the entire student linked list and prints a brief, single-line
                 * summary (ID, Name, GPA) for each student. Designed for quick lookups and to prevent terminal spam.
                 */
                case 1: {

                    printf("==========================================================================================="
                           "=============================\n");
                    printf("ALL STUDENT SUMMARY LIST\n");
                    printf("==========================================================================================="
                           "=============================\n");
                    printf("%-25s%-50s%-10s\n", "STUDENT ID", "STUDENT NAME", "GPA");
                    printf("-------------------------------------------------------------------------------------------"
                           "-----------------------------\n");

                    student *current_student = student_head;

                    while (current_student != NULL) {

                        printf("%-25u%-50.50s%-10.2f\n", current_student->id, current_student->name,
                               current_student->GPA);
                        current_student = current_student->next;
                    }

                    break;
                }

                /*
                 * DISPLAY FULL TRANSCRIPTS: Iterates through all registered students and prints a detailed transcript
                 * table for each. Includes a safety check to skip students with no enrolled courses, preventing the
                 * rendering of empty, broken UI tables.
                 */
                case 2: {

                    student *current_student = student_head;

                    /*
                     * Safety check: Skip this student and move to the next if they have no course records. This
                     * prevents segfaults and keeps the terminal clean from empty tables.
                     */
                    while (current_student != NULL) {

                        if (current_student->records == NULL) {
                            current_student = current_student->next;
                            continue;
                        }

                        printf("======================================================================================="
                               "=================================\n");
                        printf("STUDENT TRANSCRIPT     |     ID: %u     |     NAME: %s     |     GPA: %.2f\n",
                               current_student->id, current_student->name, current_student->GPA);
                        printf("======================================================================================="
                               "=================================\n");
                        printf("%-14s %-25s %-10s %-40s  %-8s  %-8s  %-8s\n", "COURSE ID", "COURSE NAME", "CREDIT",
                               "          EXAM SCORES", "AVG", "GRADE", "STATUS");
                        printf("---------------------------------------------------------------------------------------"
                               "---------------------------------\n");

                        enrollment *current_enrollment = current_student->records;

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
                                    snprintf(temp_buffer, sizeof(temp_buffer), "%s: %d  ", current_exam->exam_name,
                                             score);
                                }

                                if (strlen(exam_buffer) > 40) {
                                    break;
                                }

                                strcat(exam_buffer, temp_buffer);

                                current_exam = current_exam->next;
                                exam_index++;
                            }

                            printf("%-40.40s", exam_buffer);

                            if (current_enrollment->course_average == -1) {
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

                        printf("\n\n");
                        current_student = current_student->next;
                    }

                    break;
                }

                /*
                 * DISPLAY ALL PASSED COURSES: Iterates through the entire student list and prints detailed transcripts
                 * exclusively for passed courses. Implements strict empty-record handling and two-pass
                 * validation to keep the CLI clean.
                 */
                case 3: {

                    student *current_student = student_head;

                    while (current_student != NULL) {

                        // Primary safety check: Skips the student entirely if they have no enrolled courses.
                        if (current_student->records == NULL) {
                            current_student = current_student->next;
                            continue;
                        }

                        int passed_course_check = 0;

                        enrollment *current_enrollment = current_student->records;

                        while (current_enrollment != NULL) {

                            if (strcmp(current_enrollment->letter_grade, "--") != 0 &&
                                strcmp(current_enrollment->letter_grade, "FF") != 0) {

                                passed_course_check = 1;
                            }

                            current_enrollment = current_enrollment->next;
                        }

                        /*
                         * Two-pass validation: Skips the student if they have enrolled courses but NO passed courses.
                         * Prevents printing empty transcript headers.
                         */
                        if (!passed_course_check) {
                            current_student = current_student->next;
                            continue;
                        }

                        printf("======================================================================================="
                               "=================================\n");
                        printf("STUDENT TRANSCRIPT     |     ID: %u     |     NAME: %s     |     GPA: %.2f\n",
                               current_student->id, current_student->name, current_student->GPA);
                        printf("======================================================================================="
                               "=================================\n");
                        printf("%-14s %-25s %-10s %-40s  %-8s  %-8s  %-8s\n", "COURSE ID", "COURSE NAME", "CREDIT",
                               "          EXAM SCORES", "AVG", "GRADE", "STATUS");
                        printf("---------------------------------------------------------------------------------------"
                               "---------------------------------\n");

                        current_enrollment = current_student->records;

                        while (current_enrollment != NULL) {

                            /*
                             * Fast-forward pointer: Skips courses that are PENDING ("--") or FAILED. Stops only at
                             * passed courses to be printed.
                             */
                            while (current_enrollment != NULL &&
                                   (strcmp(current_enrollment->letter_grade, "--") == 0 ||
                                    strcmp(current_enrollment->letter_grade, "FF") == 0)) {

                                current_enrollment = current_enrollment->next;
                            }

                            if (current_enrollment == NULL) {
                                break;
                            }

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
                                    snprintf(temp_buffer, sizeof(temp_buffer), "%s: %d  ", current_exam->exam_name,
                                             score);
                                }

                                if (strlen(exam_buffer) > 40) {
                                    break;
                                }

                                strcat(exam_buffer, temp_buffer);

                                current_exam = current_exam->next;
                                exam_index++;
                            }

                            printf("%-40.40s", exam_buffer);

                            if (current_enrollment->course_average == -1) {
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

                        printf("\n\n");
                        current_student = current_student->next;
                    }

                    break;
                }
                }
            }
        }
        }
    }
}
