#include "../headers/safe_input.h"
#include "../headers/student_management_system.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void display_student(student *student_head, course *course_head) {

    if (student_head == NULL) {
        printf("\n!ERROR! The student list is empty\n\n");
        return;
    }

    if (course_head == NULL) {
        printf("\n!ERROR! The course list is empty\n\n");
        return;
    }

    int loop_flag = 1;

    while (loop_flag) {

        printf("\n\n========================================================================================="
               "===============================\n");
        printf("                                                    DISPLAY STUDENTS");
        printf("\n========================================================================================="
               "===============================\n\n");
        int choice = get_safe_int_between(0, 6, 3,
                                          "\n[0] EXIT"
                                          "\n[1] DISPLAY A STUDENT (BY ID)"
                                          "\n[2] DISPLAY ALL STUDENTS"
                                          "\n[3] DISPLAY STUDENTS IN A COURSE"
                                          "\n[4] DISPLAY CLASS AVERAGE"
                                          "\n[5] DISPLAY AVERAGE GPA"
                                          "\n[6] ADVANCED STUDENT SEARCH\n"
                                          "\nEnter the operation you want to perform: ");

        switch (choice) {
        case 0:
            printf("\nOperation cancelled. Returning to the main menu...\n\n");
            loop_flag = 0;
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

                printf("\n\n========================================================================================="
                       "===============================\n");
                printf("                                                   DISPLAY A STUDENTS");
                printf("\n========================================================================================="
                       "===============================\n\n");
                int mode = get_safe_int_between(0, 4, 3,
                                                "\n[0] EXIT"
                                                "\n[1] DISPLAY ALL COURSES"
                                                "\n[2] DISPLAY PASSED COURSES"
                                                "\n[3] DISPLAY FAILED COURSES"
                                                "\n[4] DISPLAY PENDING COURSES\n"
                                                "\nEnter the operation you want to perform: ");

                switch (mode) {

                case 0:
                    printf("\nOperation cancelled. Returning to the previous menu...\n\n");
                    loop_flag_2 = 0;
                    break;

                /*
                 * Iterates through the selected student's enrollment records and prints a formatted transcript table.
                 * Displays all courses' IDs, names, exam scores, course averages, grades and statuses
                 */
                case 1: {

                    printf("\n\n======================================================================================="
                           "===="
                           "=============================\n");
                    printf("STUDENT TRANSCRIPT     |     ID: %u     |     NAME: %s     |     GPA: ",
                           selected_student->id, selected_student->name);
                    print_gpa(selected_student->GPA);

                    printf(
                        "\n==========================================================================================="
                        "=============================\n\n\n");
                    printf("%-14s%-25s%-10s%-49s%-8s%-8s%-8s\n", "COURSE ID", "COURSE NAME", "CREDIT",
                           "          EXAM GRADES", "AVG", "GRADE", "STATUS");
                    printf("-------------------------------------------------------------------------------------------"
                           "-----------------------------\n");

                    enrollment *current_enrollment = selected_student->records;

                    while (current_enrollment != NULL) {

                        char exam_buffer[75] = "";
                        char temp_buffer[75] = "";

                        printf("%-14.14s", current_enrollment->course->course_id);
                        printf("%-25.25s", current_enrollment->course->course_name);
                        printf("%-10d", current_enrollment->course->course_credit);

                        exam_template *current_exam = current_enrollment->course->exams;
                        int exam_index = 0;

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

                        printf("%-49.49s", exam_buffer);

                        if (current_enrollment->course_average == -1) {
                            printf("%-8.8s", "N/A");
                        } else {
                            printf("%-8.2f", current_enrollment->course_average);
                        }

                        if (strcmp(current_enrollment->letter_grade, "--") == 0) {
                            printf("%-8.8s", "N/A");
                        } else {
                            printf("%-8.8s", current_enrollment->letter_grade);
                        }

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
                    break;
                }

                /*
                 * Iterates through the records to check if the student has any passed courses. If there are none,
                 * prints a warning and breaks before printing the table headers.
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
                        printf("\n!ERROR! There are no passed courses for the student with ID '%u'\n\n", selected_student->id);
                        break;
                    }

                    printf("\n\n======================================================================================="
                           "===="
                           "=============================\n");
                    printf("STUDENT TRANSCRIPT     |     ID: %u     |     NAME: %s     |     GPA: ",
                           selected_student->id, selected_student->name);
                    print_gpa(selected_student->GPA);

                    printf(
                        "\n==========================================================================================="
                        "=============================\n\n\n");
                    printf("%-14s%-25s%-10s%-49s%-8s%-8s%-8s\n", "COURSE ID", "COURSE NAME", "CREDIT",
                           "          EXAM GRADES", "AVG", "GRADE", "STATUS");
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

                        printf("%-14.14s", current_enrollment->course->course_id);
                        printf("%-25.25s", current_enrollment->course->course_name);
                        printf("%-10d", current_enrollment->course->course_credit);

                        exam_template *current_exam = current_enrollment->course->exams;
                        int exam_index = 0;

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

                        printf("%-49.49s", exam_buffer);
                        printf("%-8.2f", current_enrollment->course_average);
                        printf("%-8.8s", current_enrollment->letter_grade);
                        printf("%-8.8s\n", "PASSED");

                        current_enrollment = current_enrollment->next;
                    }

                    printf("\n\n");
                    break;
                }
                /*
                 * Iterates through the records to check if the student has any failed courses ("FF"). If there are
                 * none, prints a warning and breaks without drawing the table.
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
                        printf("\n!ERROR! There are no failed courses for the student with ID '%u'\n\n", selected_student->id);
                        break;
                    }

                    printf("\n\n======================================================================================="
                           "===="
                           "=============================\n");
                    printf("STUDENT TRANSCRIPT     |     ID: %u     |     NAME: %s     |     GPA: ",
                           selected_student->id, selected_student->name);
                    print_gpa(selected_student->GPA);

                    printf(
                        "\n==========================================================================================="
                        "=============================\n\n\n");
                    printf("%-14s%-25s%-10s%-49s%-8s%-8s%-8s\n", "COURSE ID", "COURSE NAME", "CREDIT",
                           "          EXAM GRADES", "AVG", "GRADE", "STATUS");
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

                        printf("%-14.14s", current_enrollment->course->course_id);
                        printf("%-25.25s", current_enrollment->course->course_name);
                        printf("%-10d", current_enrollment->course->course_credit);

                        exam_template *current_exam = current_enrollment->course->exams;
                        int exam_index = 0;

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

                        printf("%-49.49s", exam_buffer);
                        printf("%-8.2f", current_enrollment->course_average);
                        printf("%-8.8s", current_enrollment->letter_grade);
                        printf("%-8.8s\n", "FAILED");

                        current_enrollment = current_enrollment->next;
                    }

                    printf("\n\n");
                    break;
                }

                case 4: {

                    enrollment *current_enrollment = selected_student->records;
                    int pending_course_check = 0;

                    while (current_enrollment != NULL) {

                        if (strcmp(current_enrollment->letter_grade, "--") == 0) {
                            pending_course_check = 1;
                        }

                        current_enrollment = current_enrollment->next;
                    }

                    if (!pending_course_check) {
                        printf("\n!ERROR! There are no pending courses for the student with ID '%u'\n\n", selected_student->id);
                        break;
                    }

                    printf("\n\n======================================================================================="
                           "===="
                           "=============================\n");
                    printf("STUDENT TRANSCRIPT     |     ID: %u     |     NAME: %s     |     GPA: ",
                           selected_student->id, selected_student->name);
                    print_gpa(selected_student->GPA);

                    printf(
                        "\n==========================================================================================="
                        "=============================\n\n\n");
                    printf("%-14s%-25s%-10s%-49s%-8s%-8s%-8s\n", "COURSE ID", "COURSE NAME", "CREDIT",
                           "          EXAM GRADES", "AVG", "GRADE", "STATUS");
                    printf("-------------------------------------------------------------------------------------------"
                           "-----------------------------\n");

                    current_enrollment = selected_student->records;

                    while (current_enrollment != NULL) {

                        while (current_enrollment != NULL && (strcmp(current_enrollment->letter_grade, "--") != 0)) {
                            current_enrollment = current_enrollment->next;
                        }

                        if (current_enrollment == NULL) {
                            break;
                        }

                        char exam_buffer[75] = "";
                        char temp_buffer[75] = "";

                        printf("%-14.14s", current_enrollment->course->course_id);
                        printf("%-25.25s", current_enrollment->course->course_name);
                        printf("%-10d", current_enrollment->course->course_credit);

                        exam_template *current_exam = current_enrollment->course->exams;
                        int exam_index = 0;

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

                        printf("%-49.49s", exam_buffer);
                        printf("%-8.8s", "N/A");
                        printf("%-8.8s", "N/A");
                        printf("%-8.8s\n", "PENDING");

                        current_enrollment = current_enrollment->next;
                    }

                    printf("\n\n");
                    break;
                }
                }
            }

            break;
        }

        case 2: {

            int loop_flag_2 = 1;

            while (loop_flag_2) {

                printf("\n\n========================================================================================="
                       "===============================\n");
                printf("                                                  DISPLAY ALL STUDENTS");
                printf("\n========================================================================================="
                       "===============================\n\n");

                int mode = get_safe_int_between(0, 5, 3,
                                                "\n[0] EXIT"
                                                "\n[1] DISPLAY SUMMARY TABLE"
                                                "\n[2] DISPLAY FULL TRANSCRIPTS"
                                                "\n[3] DISPLAY ONLY PASSED COURSES"
                                                "\n[4] DISPLAY ONLY FAILED COURSES"
                                                "\n[5] DISPLAY ONLY PENDING COURSES\n"
                                                "\nEnter the operation you want to perform: ");

                switch (mode) {

                case 0:
                    printf("\nOperation cancelled. Returning to the previous menu...\n\n");
                    loop_flag_2 = 0;
                    break;

                /*
                 * Iterates through the entire student linked list and prints a brief, single-line summary (ID, Name,
                 * GPA) for each student. Designed for quick lookups and to prevent terminal spam.
                 */
                case 1: {

                    printf("\n\n======================================================================================="
                           "===="
                           "=============================\n");
                    printf("ALL STUDENT SUMMARY LIST\n");
                    printf("==========================================================================================="
                           "=============================\n\n\n");
                    printf("%-25s%-50s%-8s\n", "STUDENT ID", "STUDENT NAME", "GPA");
                    printf("-------------------------------------------------------------------------------------------"
                           "-----------------------------\n");

                    student *current_student = student_head;

                    while (current_student != NULL) {

                        printf("%-25u%-50.50s", current_student->id, current_student->name);
                        print_gpa(current_student->GPA);
                        printf("\n");
                        current_student = current_student->next;
                    }

                    printf("\n\n");
                    break;
                }

                /*
                 * Iterates through all registered students and prints a detailed transcript table for each. Includes a
                 * safety check to skip students with no enrolled courses, preventing the rendering of empty, broken UI
                 * tables.
                 */
                case 2: {

                    student *current_student = student_head;

                    /*
                     * Skip this student and move to the next if they have no course records. This prevents segfaults
                     * and keeps the terminal clean from empty tables.
                     */
                    while (current_student != NULL) {

                        if (current_student->records == NULL) {
                            current_student = current_student->next;
                            continue;
                        }

                        printf("\n\n==================================================================================="
                               "=====================================\n");
                        printf("STUDENT TRANSCRIPT     |     ID: %u     |     NAME: %s     |     GPA: ",
                               current_student->id, current_student->name);
                        print_gpa(current_student->GPA);

                        printf(
                            "\n======================================================================================="
                            "=================================\n\n\n");
                        printf("%-14s%-25s%-10s%-49s%-8s%-8s%-8s\n", "COURSE ID", "COURSE NAME", "CREDIT",
                               "          EXAM GRADES", "AVG", "GRADE", "STATUS");
                        printf("---------------------------------------------------------------------------------------"
                               "---------------------------------\n");

                        enrollment *current_enrollment = current_student->records;

                        while (current_enrollment != NULL) {

                            char exam_buffer[75] = "";
                            char temp_buffer[75] = "";

                            printf("%-14.14s", current_enrollment->course->course_id);
                            printf("%-25.25s", current_enrollment->course->course_name);
                            printf("%-10d", current_enrollment->course->course_credit);

                            exam_template *current_exam = current_enrollment->course->exams;
                            int exam_index = 0;

                            while (current_exam != NULL) {

                                int score = current_enrollment->scores[exam_index];

                                if (score == -1) {
                                    snprintf(temp_buffer, sizeof(temp_buffer), "%s: N/A  ", current_exam->exam_name);

                                } else {
                                    snprintf(temp_buffer, sizeof(temp_buffer), "%s: %d  ", current_exam->exam_name,
                                             score);
                                }

                                if (strlen(exam_buffer) > 50) {
                                    break;
                                }

                                strcat(exam_buffer, temp_buffer);

                                current_exam = current_exam->next;
                                exam_index++;
                            }

                            printf("%-49.49s", exam_buffer);

                            if (current_enrollment->course_average == -1) {
                                printf("%-8.8s", "N/A");
                            } else {
                                printf("%-8.2f", current_enrollment->course_average);
                            }

                            if (strcmp(current_enrollment->letter_grade, "--") == 0) {
                                printf("%-8.8s", "N/A");
                            } else {
                                printf("%-8.8s", current_enrollment->letter_grade);
                            }

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
                 * Iterates through the entire student list and prints detailed transcripts exclusively for passed
                 * courses. Implements strict empty-record handling and two-pass validation to keep the CLI clean.
                 */
                case 3: {

                    student *current_student = student_head;
                    int any_student_printed = 0;

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
                         * Skips the student if they have enrolled courses but NO passed courses. Prevents printing
                         * empty transcript headers.
                         */
                        if (!passed_course_check) {
                            current_student = current_student->next;
                            continue;
                        }

                        any_student_printed = 1;

                        printf("\n\n==================================================================================="
                               "===="
                               "=================================\n");
                        printf("STUDENT TRANSCRIPT     |     ID: %u     |     NAME: %s     |     GPA: ",
                               current_student->id, current_student->name);
                        print_gpa(current_student->GPA);

                        printf(
                            "\n======================================================================================="
                            "=================================\n\n\n");
                        printf("%-14s%-25s%-10s%-49s%-8s%-8s%-8s\n", "COURSE ID", "COURSE NAME", "CREDIT",
                               "          EXAM GRADES", "AVG", "GRADE", "STATUS");
                        printf("---------------------------------------------------------------------------------------"
                               "---------------------------------\n");

                        current_enrollment = current_student->records;

                        while (current_enrollment != NULL) {

                            // Skips courses that are PENDING ("--") or FAILED. Stops only at passed courses to be
                            // printed.
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

                            printf("%-14.14s", current_enrollment->course->course_id);
                            printf("%-25.25s", current_enrollment->course->course_name);
                            printf("%-10d", current_enrollment->course->course_credit);

                            exam_template *current_exam = current_enrollment->course->exams;
                            int exam_index = 0;

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

                            printf("%-8.2f", current_enrollment->course_average);
                            printf("%-8.8s", current_enrollment->letter_grade);
                            printf("%-8.8s\n", "PASSED");

                            current_enrollment = current_enrollment->next;
                        }

                        printf("\n\n");
                        current_student = current_student->next;
                    }

                    if (!any_student_printed) {
                        printf("\n!ERROR! There are no students who have passed any courses\n\n");
                    }

                    break;
                }

                /*
                 * Iterates through all students in the system. Uses two-pass validation to completely skip students who
                 * have no courses or no failed ("FF") courses.
                 */
                case 4: {

                    student *current_student = student_head;
                    int any_student_printed = 0;

                    while (current_student != NULL) {

                        if (current_student->records == NULL) {
                            current_student = current_student->next;
                            continue;
                        }

                        int failed_course_check = 0;

                        enrollment *current_enrollment = current_student->records;

                        // Checks if the student has at least one failed course.
                        while (current_enrollment != NULL) {

                            if (strcmp(current_enrollment->letter_grade, "FF") == 0) {
                                failed_course_check = 1;
                            }

                            current_enrollment = current_enrollment->next;
                        }

                        if (!failed_course_check) {
                            current_student = current_student->next;
                            continue;
                        }

                        any_student_printed = 1;

                        printf("\n\n==================================================================================="
                               "===="
                               "=================================\n");
                        printf("STUDENT TRANSCRIPT     |     ID: %u     |     NAME: %s     |     GPA: ",
                               current_student->id, current_student->name);
                        print_gpa(current_student->GPA);

                        printf(
                            "\n======================================================================================="
                            "=================================\n\n\n");
                        printf("%-14s%-25s%-10s%-49s%-8s%-8s%-8s\n", "COURSE ID", "COURSE NAME", "CREDIT",
                               "          EXAM GRADES", "AVG", "GRADE", "STATUS");
                        printf("---------------------------------------------------------------------------------------"
                               "---------------------------------\n");

                        current_enrollment = current_student->records;

                        while (current_enrollment != NULL) {

                            // Skips courses that are PASSED or PENDING, stopping only at "FF" grades.
                            while (current_enrollment != NULL && strcmp(current_enrollment->letter_grade, "FF") != 0) {

                                current_enrollment = current_enrollment->next;
                            }

                            if (current_enrollment == NULL) {
                                break;
                            }

                            char exam_buffer[75] = "";
                            char temp_buffer[75] = "";

                            printf("%-14.14s", current_enrollment->course->course_id);
                            printf("%-25.25s", current_enrollment->course->course_name);
                            printf("%-10d", current_enrollment->course->course_credit);

                            exam_template *current_exam = current_enrollment->course->exams;
                            int exam_index = 0;

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

                            printf("%-49.49s", exam_buffer);
                            printf("%-8.2f", current_enrollment->course_average);
                            printf("%-8.8s", current_enrollment->letter_grade);
                            printf("%-8.8s\n", "FAILED");

                            current_enrollment = current_enrollment->next;
                        }

                        printf("\n\n");
                        current_student = current_student->next;
                    }

                    if (!any_student_printed) {
                        printf("\n!ERROR! There are no students who have failed any courses\n\n");
                    }

                    break;
                }

                /*
                 * Iterates through all students in the system. Uses two-pass validation to completely skip students who
                 * have no courses or no pending ("--") courses.
                 */
                case 5: {

                    student *current_student = student_head;
                    int any_student_printed = 0;

                    while (current_student != NULL) {

                        if (current_student->records == NULL) {
                            current_student = current_student->next;
                            continue;
                        }

                        int pending_course_check = 0;

                        enrollment *current_enrollment = current_student->records;

                        // Checks if the student has at least one pending course.
                        while (current_enrollment != NULL) {

                            if (strcmp(current_enrollment->letter_grade, "--") == 0) {
                                pending_course_check = 1;
                            }

                            current_enrollment = current_enrollment->next;
                        }

                        if (!pending_course_check) {
                            current_student = current_student->next;
                            continue;
                        }

                        any_student_printed = 1;

                        printf("\n\n==================================================================================="
                               "===="
                               "=================================\n");
                        printf("STUDENT TRANSCRIPT     |     ID: %u     |     NAME: %s     |     GPA: ",
                               current_student->id, current_student->name);
                        print_gpa(current_student->GPA);

                        printf(
                            "\n======================================================================================="
                            "=================================\n\n\n");
                        printf("%-14s%-25s%-10s%-49s%-8s%-8s%-8s\n", "COURSE ID", "COURSE NAME", "CREDIT",
                               "          EXAM GRADES", "AVG", "GRADE", "STATUS");
                        printf("---------------------------------------------------------------------------------------"
                               "---------------------------------\n");

                        current_enrollment = current_student->records;

                        while (current_enrollment != NULL) {

                            // Skips courses that are PASSED or FAILED, stopping only at "--" grades.
                            while (current_enrollment != NULL &&
                                   (strcmp(current_enrollment->letter_grade, "--") != 0)) {

                                current_enrollment = current_enrollment->next;
                            }

                            if (current_enrollment == NULL) {
                                break;
                            }

                            char exam_buffer[75] = "";
                            char temp_buffer[75] = "";

                            printf("%-14.14s", current_enrollment->course->course_id);
                            printf("%-25.25s", current_enrollment->course->course_name);
                            printf("%-10d", current_enrollment->course->course_credit);

                            exam_template *current_exam = current_enrollment->course->exams;
                            int exam_index = 0;

                            while (current_exam != NULL) {

                                int score = current_enrollment->scores[exam_index];

                                if (score == -1) {
                                    snprintf(temp_buffer, sizeof(temp_buffer), "%s: N/A  ", current_exam->exam_name);

                                } else {
                                    snprintf(temp_buffer, sizeof(temp_buffer), "%s: %d  ", current_exam->exam_name,
                                             score);
                                }

                                if (strlen(exam_buffer) > 50) {
                                    break;
                                }

                                strcat(exam_buffer, temp_buffer);

                                current_exam = current_exam->next;
                                exam_index++;
                            }

                            printf("%-49.49s", exam_buffer);
                            printf("%-8.8s", "N/A");
                            printf("%-8.8s", "N/A");
                            printf("%-8.8s\n", "PENDING");

                            current_enrollment = current_enrollment->next;
                        }

                        printf("\n\n");
                        current_student = current_student->next;
                    }

                    if (!any_student_printed) {
                        printf("\n!ERROR! There are no students who have any pending courses\n\n");
                    }

                    break;
                }
                }
            }

            break;
        }

        /*
         * Course-specific student viewer. Allows the user to select a course and apply various filters (passed, failed,
         * pending, or full transcript).
         */
        case 3: {

            print_all_courses(course_head);

            char selected_course_id[10];

            get_safe_string(3, selected_course_id, sizeof(selected_course_id),
                            "\nEnter the course ID you want to display (or type 'exit' to cancel): ");

            for (int i = 0; selected_course_id[i] != '\0'; i++) {
                selected_course_id[i] = toupper((unsigned char)selected_course_id[i]);
            }

            if (strcmp(selected_course_id, "EXIT") == 0) {
                printf("\nOperation cancelled. Returning to the previous menu...\n\n");
                break;
            }

            course *selected_course = course_head;

            while (selected_course != NULL) {

                if (strcmp(selected_course->course_id, selected_course_id) == 0) {
                    break;
                }

                selected_course = selected_course->next;
            }

            if (selected_course == NULL) {
                printf("\nThere is no course with ID '%s' in the course list\n\n", selected_course_id);
                break;
            }

            int loop_flag_2 = 1;

            while (loop_flag_2) {

                printf("\n\n========================================================================================="
                       "===============================\n");
                printf("                                             DISPLAY STUDENTS IN A COURSE");
                printf("\n========================================================================================="
                       "===============================\n\n");

                int mode = get_safe_int_between(0, 5, 3,
                                                "\n[0] EXIT"
                                                "\n[1] DISPLAY ALL ENROLLED STUDENTS"
                                                "\n[2] DISPLAY ONLY PASSED STUDENTS"
                                                "\n[3] DISPLAY ONLY FAILED STUDENTS"
                                                "\n[4] DISPLAY ONLY PENDING STUDENTS\n"
                                                "\n[5] DISPLAY ALL ENROLLED STUDENTS' TRANSCRIPTS");

                switch (mode) {

                case 0:
                    printf("\nOperation cancelled. Returning to the previous menu...\n\n");
                    loop_flag_2 = 0;
                    break;
                /*
                 * Iterates through the student list to find and print exam grades for everyone taking the selected
                 * course. Uses a lazy-header approach  to prevent drawing empty tables if the course has no students.
                 */
                case 1: {

                    student *current_student = student_head;
                    int header_printed = 0;

                    while (current_student != NULL) {

                        enrollment *current_enrollment = current_student->records;

                        while (current_enrollment != NULL) {

                            if (strcmp(current_enrollment->course->course_id, selected_course_id) == 0) {
                                break;
                            }

                            current_enrollment = current_enrollment->next;
                        }

                        // Skip the student if they are not enrolled in the selected course.
                        if (current_enrollment == NULL) {
                            current_student = current_student->next;
                            continue;
                        }

                        // Prints the table headers only once, right before the first valid record.
                        if (!header_printed) {
                            printf("%-15s%-30s%-45s%-8s%-8s%-8s%-8s\n", "STUDENT ID", "STUDENT NAME", "EXAM GRADES",
                                   "AVG", "GRADE", "GPA", "STATUS");
                            printf("-----------------------------------------------------------------------------------"
                                   "-------------------------------------\n");
                            header_printed = 1;
                        }

                        printf("%-15u", current_student->id);
                        printf("%-30.30s", current_student->name);

                        exam_template *current_exam = current_enrollment->course->exams;

                        char exam_buffer[75] = "";
                        char temp_buffer[75] = "";
                        int exam_index = 0;

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

                        printf("%-49.49s", exam_buffer);

                        if (current_enrollment->course_average == -1) {
                            printf("%-8.8s", "N/A");
                        } else {
                            printf("%-8.2f", current_enrollment->course_average);
                        }

                        if (strcmp(current_enrollment->letter_grade, "--") == 0) {
                            printf("%-8.8s", "N/A");
                        } else {
                            printf("%-8.8s", current_enrollment->letter_grade);
                        }

                        print_gpa(current_student->GPA);

                        if (strcmp(current_enrollment->letter_grade, "--") == 0) {
                            printf("%-8.8s\n", "PENDING");
                        } else if (strcmp(current_enrollment->letter_grade, "FF") == 0) {
                            printf("%-8.8s\n", "FAILED");
                        } else {
                            printf("%-8.8s\n", "PASSED");
                        }

                        current_student = current_student->next;
                    }

                    // If the header flag is still 0, it means no one is enrolled in this course.
                    if (!header_printed) {
                        printf("\n!ERROR! There are no students enrolled in the course '%s'\n\n", selected_course_id);
                    }

                    printf("\n\n");
                    break;
                }

                // Filters and prints only the students who have successfully passed the selected course.
                case 2: {

                    student *current_student = student_head;
                    int header_printed = 0;

                    while (current_student != NULL) {

                        enrollment *current_enrollment = current_student->records;

                        while (current_enrollment != NULL) {

                            if (strcmp(current_enrollment->course->course_id, selected_course_id) == 0) {
                                break;
                            }

                            current_enrollment = current_enrollment->next;
                        }

                        /*
                         * Safely skip if the student is not enrolled (NULL), has a pending grade ("--"), or failed the
                         * course ("FF")
                         */
                        if (current_enrollment == NULL || strcmp(current_enrollment->letter_grade, "--") == 0 ||
                            strcmp(current_enrollment->letter_grade, "FF") == 0) {
                            current_student = current_student->next;
                            continue;
                        }

                        if (!header_printed) {
                            printf("%-15s%-30s%-45s%-8s%-8s%-8s%-8s\n", "STUDENT ID", "STUDENT NAME", "EXAM GRADES",
                                   "AVG", "GRADE", "GPA", "STATUS");
                            printf("-----------------------------------------------------------------------------------"
                                   "-------------------------------------\n");
                            header_printed = 1;
                        }

                        printf("%-15u", current_student->id);
                        printf("%-30.30s", current_student->name);

                        exam_template *current_exam = current_enrollment->course->exams;

                        char exam_buffer[75] = "";
                        char temp_buffer[75] = "";
                        int exam_index = 0;

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

                        printf("%-49.49s", exam_buffer);
                        printf("%-8.2f", current_enrollment->course_average);
                        printf("%-8.8s", current_enrollment->letter_grade);
                        print_gpa(current_student->GPA);
                        printf("%-8.8s\n", "PASSED");

                        current_student = current_student->next;
                    }

                    // Handle the case where no students passed, or the course has zero enrollments.
                    if (!header_printed) {
                        printf("\n!ERROR! There are no students who passed the course '%s'\n\n", selected_course_id);
                    }

                    printf("\n\n");
                    break;
                }

                // Filters and prints only the students who have failed ("FF") the selected course.
                case 3: {

                    student *current_student = student_head;
                    int header_printed = 0;

                    while (current_student != NULL) {

                        enrollment *current_enrollment = current_student->records;

                        while (current_enrollment != NULL) {

                            if (strcmp(current_enrollment->course->course_id, selected_course_id) == 0) {
                                break;
                            }

                            current_enrollment = current_enrollment->next;
                        }

                        /*
                         * Safely skip if the student is not enrolled (NULL), has a pending grade ("--"), or passed the
                         * course.
                         */
                        if (current_enrollment == NULL || strcmp(current_enrollment->letter_grade, "FF") != 0) {
                            current_student = current_student->next;
                            continue;
                        }

                        if (!header_printed) {
                            printf("%-15s%-30s%-45s%-8s%-8s%-8s%-8s\n", "STUDENT ID", "STUDENT NAME", "EXAM GRADES",
                                   "AVG", "GRADE", "GPA", "STATUS");
                            printf("-----------------------------------------------------------------------------------"
                                   "-------------------------------------\n");
                            header_printed = 1;
                        }

                        printf("%-15u", current_student->id);
                        printf("%-30.30s", current_student->name);

                        exam_template *current_exam = current_enrollment->course->exams;

                        char exam_buffer[75] = "";
                        char temp_buffer[75] = "";
                        int exam_index = 0;

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

                        printf("%-49.49s", exam_buffer);
                        printf("%-8.2f", current_enrollment->course_average);
                        printf("%-8.8s", current_enrollment->letter_grade);
                        print_gpa(current_student->GPA);
                        printf("%-8.8s\n", "FAILED");

                        current_student = current_student->next;
                    }

                    // Handle the case where no students failed, or the course has zero enrollments.
                    if (!header_printed) {
                        printf("\n!ERROR! There are no students who failed the course '%s'\n\n", selected_course_id);
                    }

                    printf("\n\n");
                    break;
                }

                // Filters and prints only the students with pending grades ("--") for the selected course.
                case 4: {

                    student *current_student = student_head;
                    int header_printed = 0;

                    while (current_student != NULL) {

                        enrollment *current_enrollment = current_student->records;

                        while (current_enrollment != NULL) {

                            if (strcmp(current_enrollment->course->course_id, selected_course_id) == 0) {
                                break;
                            }

                            current_enrollment = current_enrollment->next;
                        }

                        /*
                         * Safely skip if the student is not enrolled (NULL), has a passed grade, or failed the
                         * course ("FF")
                         */
                        if (current_enrollment == NULL || strcmp(current_enrollment->letter_grade, "--") != 0) {
                            current_student = current_student->next;
                            continue;
                        }

                        if (!header_printed) {
                            printf("%-15s%-30s%-45s%-8s%-8s%-8s%-8s\n", "STUDENT ID", "STUDENT NAME", "EXAM GRADES",
                                   "AVG", "GRADE", "GPA", "STATUS");
                            printf("-----------------------------------------------------------------------------------"
                                   "-------------------------------------\n");
                            header_printed = 1;
                        }

                        printf("%-15u", current_student->id);
                        printf("%-30.30s", current_student->name);

                        exam_template *current_exam = current_enrollment->course->exams;

                        char exam_buffer[75] = "";
                        char temp_buffer[75] = "";
                        int exam_index = 0;

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

                        printf("%-49.49s", exam_buffer);
                        printf("%-8.8s", "N/A");
                        printf("%-8.8s", "N/A");
                        print_gpa(current_student->GPA);
                        printf("%-8.8s\n", "PENDING");

                        current_student = current_student->next;
                    }

                    // Handle the case where no students are pending, or the course has zero enrollments.
                    if (!header_printed) {
                        printf("\n!ERROR! There are no students with pending grades for the course '%s'\n\n",
                               selected_course_id);
                    }

                    printf("\n\n");
                    break;
                }

                /*
                 * Finds all students taking the selected course and prints their entire  academic transcript (all
                 * enrolled courses). Uses a dedicated flag to  handle cases where the course has zero enrollments.
                 */
                case 5: {

                    student *current_student = student_head;
                    int student_found = 0;

                    while (current_student != NULL) {

                        enrollment *current_enrollment = current_student->records;

                        /*
                         * Check if the student is actually enrolled in the selected course. If found, trigger the flag
                         * and break early to save CPU cycles.
                         */
                        while (current_enrollment != NULL) {

                            if (strcmp(current_enrollment->course->course_id, selected_course_id) == 0) {
                                student_found = 1;
                                break;
                            }

                            current_enrollment = current_enrollment->next;
                        }

                        // Skip the student if they are not enrolled in the selected course.
                        if (current_enrollment == NULL) {
                            current_student = current_student->next;
                            continue;
                        }

                        printf("\n\n==================================================================================="
                               "===="
                               "=================================\n");
                        printf("STUDENT TRANSCRIPT     |     ID: %u     |     NAME: %s     |     GPA: ",
                               current_student->id, current_student->name);
                        print_gpa(current_student->GPA);

                        printf(
                            "\n======================================================================================="
                            "=================================\n\n\n");
                        printf("%-14s%-25s%-10s%-49s%-8s%-8s%-8s\n", "COURSE ID", "COURSE NAME", "CREDIT",
                               "          EXAM GRADES", "AVG", "GRADE", "STATUS");
                        printf("---------------------------------------------------------------------------------------"
                               "---------------------------------\n");

                        /*
                         * Reset the enrollment pointer back to the head to print the student's COMPLETE transcript,
                         * not just the selected course.
                         */
                        current_enrollment = current_student->records;

                        while (current_enrollment != NULL) {

                            char exam_buffer[75] = "";
                            char temp_buffer[75] = "";

                            printf("%-14.14s", current_enrollment->course->course_id);
                            printf("%-25.25s", current_enrollment->course->course_name);
                            printf("%-10d", current_enrollment->course->course_credit);

                            exam_template *current_exam = current_enrollment->course->exams;
                            int exam_index = 0;

                            while (current_exam != NULL) {

                                int score = current_enrollment->scores[exam_index];

                                if (score == -1) {
                                    snprintf(temp_buffer, sizeof(temp_buffer), "%s: N/A  ", current_exam->exam_name);

                                } else {
                                    snprintf(temp_buffer, sizeof(temp_buffer), "%s: %d  ", current_exam->exam_name,
                                             score);
                                }

                                if (strlen(exam_buffer) > 50) {
                                    break;
                                }

                                strcat(exam_buffer, temp_buffer);

                                current_exam = current_exam->next;
                                exam_index++;
                            }

                            printf("%-49.49s", exam_buffer);

                            if (current_enrollment->course_average == -1) {
                                printf("%-8.8s", "N/A");
                            } else {
                                printf("%-8.2f", current_enrollment->course_average);
                            }

                            if (strcmp(current_enrollment->letter_grade, "--") == 0) {
                                printf("%-8.8s", "N/A");
                            } else {
                                printf("%-8.8s", current_enrollment->letter_grade);
                            }

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

                    /*
                     * Handle the edge case where the system scanned all students but found no one  taking the requested
                     * course.
                     */
                    if (!student_found) {
                        printf("\n!ERROR! There are no students enrolled in the course '%s'\n\n", selected_course_id);
                    }

                    break;
                }
                }
            }

            break;
        }

        // Provides sub-options to calculate and display the average grades for specific courses or all courses.
        case 4: {

            int loop_flag_2 = 1;

            while (loop_flag_2) {

                printf("\n\n========================================================================================="
                       "===============================\n");
                printf("                                                 DISPLAY CLASS AVERAGE");
                printf("\n========================================================================================="
                       "===============================\n\n");

                int mode = get_safe_int_between(0, 2, 3,
                                                "\n[0] EXIT"
                                                "\n[1] DISPLAY CLASS AVERAGE FOR A COURSE"
                                                "\n[2] DISPLAY CLASS AVERAGE FOR ALL COURSES\n"
                                                "\nEnter the operation you want to perform: ");

                switch (mode) {

                case 0:
                    printf("\nOperation cancelled. Returning to the main menu...\n\n");
                    loop_flag_2 = 0;
                    break;

                // Calculates the overall average of a course by summing up valid (non-pending) student averages.
                case 1: {

                    char selected_course_id[10];

                    get_safe_string(
                        3, selected_course_id, sizeof(selected_course_id),
                        "\nEnter the course ID you want to display class average (or type 'exit' to cancel): ");

                    for (int i = 0; selected_course_id[i] != '\0'; i++) {
                        selected_course_id[i] = toupper((unsigned char)selected_course_id[i]);
                    }

                    if (strcmp(selected_course_id, "EXIT") == 0) {
                        printf("\nOperation cancelled. Returning to the previous menu...\n\n");
                        break;
                    }

                    course *selected_course = course_head;

                    while (selected_course != NULL) {

                        if (strcmp(selected_course->course_id, selected_course_id) == 0) {
                            break;
                        }

                        selected_course = selected_course->next;
                    }

                    if (selected_course == NULL) {
                        printf("\nThere is no course with ID '%s' in the course list\n\n", selected_course_id);
                        break;
                    }

                    float total_average = 0;
                    int student_counter = 0;
                    int enrolled_student_found = 0;

                    student *current_student = student_head;

                    /*
                     * Iterate through all students to find valid enrollments for the selected course. Students with
                     * pending grades (average == -1) are explicitly excluded to prevent mathematical errors and data
                     * skewing.
                     */
                    while (current_student != NULL) {

                        enrollment *current_enrollment = current_student->records;

                        while (current_enrollment != NULL) {

                            if (strcmp(current_enrollment->course->course_id, selected_course_id) == 0 &&
                                current_enrollment->course_average != -1) {
                                student_counter++;
                                enrolled_student_found = 1;
                                break;
                            }

                            current_enrollment = current_enrollment->next;
                        }

                        if (current_enrollment == NULL) {
                            current_student = current_student->next;
                            continue;
                        }

                        total_average += current_enrollment->course_average;

                        current_student = current_student->next;
                    }

                    // Checks if any valid students were found before division.
                    if (!enrolled_student_found) {
                        printf("\n!ERROR! There are no students enrolled, or no course averages have been calculated for the "
                               "course '%s'\n\n",
                               selected_course_id);
                        break;
                    }

                    float course_average = total_average / student_counter;

                    printf("\n\n======================================================================================="
                           "=================================\n");
                    printf("Course average for '%s': %.2f\n", selected_course_id, course_average);
                    printf("======================================================================================="
                           "=================================\n");

                    printf("\n\n");
                    break;
                }

                /*
                 * Iterates through the entire course list and calculates the overall average for each course by
                 * scanning all valid student records.
                 */
                case 2: {

                    course *current_course = course_head;

                    int header_printed = 0;
                    int course_avg_printed = 0;

                    while (current_course != NULL) {

                        int enrolled_student_found = 0;
                        float total_average = 0;
                        int student_counter = 0;

                        student *current_student = student_head;

                        while (current_student != NULL) {

                            enrollment *current_enrollment = current_student->records;

                            /*
                             * Match the student's enrollment with the current course in the outer loop. Exclude
                             * students with pending grades (-1) to ensure accurate mathematical averages.
                             */
                            while (current_enrollment != NULL) {

                                if (strcmp(current_enrollment->course->course_id, current_course->course_id) == 0 &&
                                    current_enrollment->course_average != -1) {
                                    student_counter++;
                                    enrolled_student_found = 1;
                                    break;
                                }

                                current_enrollment = current_enrollment->next;
                            }

                            if (current_enrollment == NULL) {
                                current_student = current_student->next;
                                continue;
                            }

                            total_average += current_enrollment->course_average;

                            current_student = current_student->next;
                        }

                        /*
                         * If a course has zero enrollments or no calculated grades yet, skip it completely to prevent
                         * Divide-by-Zero exceptions and keep the output clean.
                         */
                        if (!enrolled_student_found) {
                            current_course = current_course->next;
                            continue;
                        }

                        if (!header_printed) {
                            printf("\n\n%-20s%-40s%-20s \n", "COURSE ID", "COURSE NAME", "COURSE AVG");
                            printf("-----------------------------------------------------------------------------------"
                                   "-------------------------------------\n");

                            header_printed = 1;
                        }

                        float course_average = total_average / student_counter;

                        printf("%-20s%-40.40s%-20.2f\n", current_course->course_id, current_course->course_name,
                               course_average);
                        course_avg_printed = 1;

                        current_course = current_course->next;
                    }

                    if (!course_avg_printed) {
                        printf("\n!ERROR! There are no students enrolled, or no course averages have been calculated for any "
                               "courses\n\n");
                    }

                    printf("\n\n");
                    break;
                }
                }
            }

            break;
        }

        // Calculates the global GPA average of the entire institution.
        case 5: {

            float total_GPA = 0;
            int student_counter = 0;
            student *current_student = student_head;

            // Iterate through the entire student database to accumulate valid GPAs.
            while (current_student != NULL) {

                /*
                 * Exclude students whose GPAs are not yet calculated (-1.0) to prevent skewing the overall average
                 * downwards.
                 */
                if (current_student->GPA == -1) {
                    current_student = current_student->next;
                    continue;
                }

                total_GPA += current_student->GPA;
                student_counter++;

                current_student = current_student->next;
            }

            // Ensures at least one valid GPA exists before performing division.
            if (!student_counter) {
                printf("\n!ERROR! There are no students with calculated GPA in the list\n\n");
                break;
            }

            float average_GPA = total_GPA / student_counter;

            printf("\n\n======================================================================================="
                   "=================================\n");
            printf("Average GPA for student list: %.2f\n", average_GPA);
            printf("======================================================================================="
                   "=================================\n");

            printf("\n\n");
            break;
        }

        case 6: {

            int loop_flag_2 = 1;

            while (loop_flag_2) {

                printf("\n\n========================================================================================="
                       "===============================\n");
                printf("                                               ADVANCED STUDENT SEARCH");
                printf("\n========================================================================================="
                       "===============================\n\n");

                int mode = get_safe_int_between(0, 6, 3,
                                                "\n[0] EXIT"
                                                "\n[1] FIND TOP STUDENTS BY GPA"
                                                "\n[2] FIND TOP STUDENTS BY GPA IN A COURSE"
                                                "\n[3] FIND STUDENTS BY LETTER GRADE IN A COURSE"
                                                "\n[4] FIND STUDENTS BY COURSE AVERAGE RANGE IN A COURSE"
                                                "\n[5] FIND STUDENTS BY GPA RANGE"
                                                "\n[6] SEARCH STUDENTS BY NAME\n"
                                                "\nEnter the operation you want to perform: ");

                switch (mode) {

                case 0:
                    printf("\nOperation cancelled. Returning to the previous menu...\n");
                    loop_flag_2 = 0;
                    break;

                /*
                 * Find Top Students by GPA Uses a two-pass algorithm. First pass finds the highest GPA in the system.
                 * Second pass prints all students who share that top score (handles ties perfectly).
                 */
                case 1: {

                    // Iterates through the entire student database to find the maximum calculated GPA.
                    float TOP_GPA = -1.0;

                    student *current_student = student_head;

                    while (current_student != NULL) {

                        if (current_student->GPA > TOP_GPA) {
                            TOP_GPA = current_student->GPA;
                        }

                        current_student = current_student->next;
                    }

                    /*
                     * Edge Case Protection: If the max GPA is still -1.0, it means nobody has a calculated GPA yet.
                     * Break early to prevent drawing empty tables.
                     */
                    if (TOP_GPA == -1.0) {
                        printf("\n!ERROR! There are no students with calculated GPA in the list\n\n");
                        break;
                    }

                    printf("\n\n======================================================================================="
                           "===="
                           "=============================\n");
                    printf("TOP STUDENTS BY GPA\n");
                    printf("==========================================================================================="
                           "=============================\n\n\n");
                    printf("%-25s%-50s%-8s\n", "STUDENT ID", "STUDENT NAME", "GPA");
                    printf("-------------------------------------------------------------------------------------------"
                           "-----------------------------\n");

                    /*
                     * Reset the pointer to the head of the list and print a summary table strictly for students
                     * matching the TOP_GPA.
                     */
                    current_student = student_head;

                    while (current_student != NULL) {

                        if (current_student->GPA == TOP_GPA) {
                            printf("%-25u%-50.50s", current_student->id, current_student->name);
                            print_gpa(current_student->GPA);
                            printf("\n");
                        }

                        current_student = current_student->next;
                    }

                    printf("\n\n");
                    break;
                }
                /*
                 * Find Top Students by GPA in a Specific Course Uses a two-pass algorithm filtered by course
                 * enrollment. Identifies the highest overall GPA among students taking the specified course,
                 * thenprints a summary of all enrolled students sharing that top GPA.
                 */
                case 2: {

                    printf("\n\nAVAILABLE COURSES:\n");
                    printf("-------------------------------------------------------------------------------------------"
                           "-----------------------------\n");
                    print_all_courses(course_head);

                    char selected_course_id[10];

                    get_safe_string(
                        3, selected_course_id, sizeof(selected_course_id),
                        "\nEnter the course ID you want to find top students by GPA (or type 'exit' to cancel): ");

                    for (int i = 0; selected_course_id[i] != '\0'; i++) {
                        selected_course_id[i] = toupper((unsigned char)selected_course_id[i]);
                    }

                    if (strcmp(selected_course_id, "EXIT") == 0) {
                        printf("\nOperation cancelled. Returning to the previous menu...\n\n");
                        break;
                    }

                    course *current_course = course_head;

                    while (current_course != NULL) {

                        if (strcmp(current_course->course_id, selected_course_id) == 0) {
                            break;
                        }

                        current_course = current_course->next;
                    }

                    if (current_course == NULL) {
                        printf("\n!ERROR! There is no course with ID '%s' in the list\n\n", selected_course_id);
                        break;
                    }

                    /*
                     * Iterates through students, checking if they are enrolled in the target course. Track the maximum
                     * overall GPA among these specific students.
                     */
                    float TOP_GPA = -1.0;

                    student *current_student = student_head;

                    while (current_student != NULL) {

                        if (current_student->records == NULL) {
                            current_student = current_student->next;
                            continue;
                        }

                        enrollment *current_enrollment = current_student->records;

                        while (current_enrollment != NULL) {

                            if (strcmp(current_enrollment->course->course_id, current_course->course_id) == 0) {
                                break;
                            }

                            current_enrollment = current_enrollment->next;
                        }

                        if (current_enrollment == NULL) {
                            current_student = current_student->next;
                            continue;
                        }

                        if (current_student->GPA > TOP_GPA) {
                            TOP_GPA = current_student->GPA;
                        }

                        current_student = current_student->next;
                    }

                    if (TOP_GPA == -1.0) {
                        printf("\n!ERROR! There are no students with calculated GPA in the course with ID '%s'\n\n",
                               current_course->course_id);
                        break;
                    }

                    printf("\n\n======================================================================================="
                           "===="
                           "=============================\n");
                    printf("TOP STUDENTS BY GPA IN '%s'\n", current_course->course_id);
                    printf("==========================================================================================="
                           "=============================\n\n\n");
                    printf("%-25s%-50s%-8s\n", "STUDENT ID", "STUDENT NAME", "GPA");
                    printf("-------------------------------------------------------------------------------------------"
                           "-----------------------------\n");
                    /*
                     * Iterates again to print the summary table for enrolled students  whose GPA matches the calculated
                     * TOP_GPA.
                     */
                    current_student = student_head;

                    while (current_student != NULL) {

                        if (current_student->records == NULL) {
                            current_student = current_student->next;
                            continue;
                        }

                        enrollment *current_enrollment = current_student->records;

                        while (current_enrollment != NULL) {

                            if (strcmp(current_enrollment->course->course_id, current_course->course_id) == 0) {
                                break;
                            }

                            current_enrollment = current_enrollment->next;
                        }

                        if (current_enrollment == NULL) {
                            current_student = current_student->next;
                            continue;
                        }

                        if (current_student->GPA == TOP_GPA) {
                            printf("%-25u%-50.50s", current_student->id, current_student->name);
                            print_gpa(current_student->GPA);
                            printf("\n");
                        }

                        current_student = current_student->next;
                    }

                    printf("\n\n");
                    break;
                }

                /*
                 * Find Students by Letter Grade in a Course Prompts the user for a course ID and a target letter grade.
                 * Performs a case-insensitive search and prints a transcript table of all  matching students. Uses a
                 * lazy-header approach to keep the UI clean.
                 */
                case 3: {

                    print_all_courses(course_head);

                    char selected_course_id[10];

                    get_safe_string(
                        3, selected_course_id, sizeof(selected_course_id),
                        "\nEnter the course ID you want to find students by letter grade (or type 'exit' to cancel): ");

                    for (int i = 0; selected_course_id[i] != '\0'; i++) {
                        selected_course_id[i] = toupper((unsigned char)selected_course_id[i]);
                    }

                    if (strcmp(selected_course_id, "EXIT") == 0) {
                        printf("\nOperation cancelled. Returning to the previous menu...\n\n");
                        break;
                    }

                    course *current_course = course_head;

                    while (current_course != NULL) {

                        if (strcmp(current_course->course_id, selected_course_id) == 0) {
                            break;
                        }

                        current_course = current_course->next;
                    }

                    if (current_course == NULL) {
                        printf("\n!ERROR! There is no course with ID '%s' in the list", selected_course_id);
                        break;
                    }

                    /*
                     * Prompt for the target letter grade and standardize it to uppercase  to ensure case-insensitive
                     * comparison (e.g., 'aa' becomes 'AA').
                     */
                    char letter_grade[5];

                    get_safe_string(3, letter_grade, sizeof(letter_grade),
                                    "\nEnter the letter grade you want to find (or type 'exit' to cancel): ");

                    for (int i = 0; letter_grade[i] != '\0'; i++) {
                        letter_grade[i] = toupper((unsigned char)letter_grade[i]);
                    }

                    if (strcmp(letter_grade, "EXIT") == 0) {
                        printf("\nOperation cancelled. Returning to the previous menu...\n\n");
                        break;
                    }

                    /*
                     * Iterate through the entire student database. Skip students with no enrollments  or those who are
                     * not taking the target course.
                     */
                    int header_printed = 0;

                    student *current_student = student_head;

                    while (current_student != NULL) {

                        if (current_student->records == NULL) {
                            current_student = current_student->next;
                            continue;
                        }

                        enrollment *current_enrollment = current_student->records;

                        while (current_enrollment != NULL) {

                            if (strcmp(current_enrollment->course->course_id, current_course->course_id) == 0) {
                                break;
                            }

                            current_enrollment = current_enrollment->next;
                        }

                        if (current_enrollment == NULL) {
                            current_student = current_student->next;
                            continue;
                        }

                        /*
                         * If the student is taking the course AND their grade matches the target, print the table
                         * header (only once) followed by their transcript record.
                         */
                        if (strcmp(current_enrollment->letter_grade, letter_grade) == 0) {

                            if (!header_printed) {
                                printf("%-15s%-30s%-45s%-8s%-8s%-8s%-8s\n", "STUDENT ID", "STUDENT NAME", "EXAM GRADES",
                                       "AVG", "GRADE", "GPA", "STATUS");
                                printf("-------------------------------------------------------------------------------"
                                       "-----------------------------------------\n");
                                header_printed = 1;
                            }

                            printf("%-15u", current_student->id);
                            printf("%-30.30s", current_student->name);

                            exam_template *current_exam = current_enrollment->course->exams;

                            char exam_buffer[75] = "";
                            char temp_buffer[75] = "";
                            int exam_index = 0;

                            while (current_exam != NULL) {

                                int score = current_enrollment->scores[exam_index];

                                if (score == -1) {
                                    snprintf(temp_buffer, sizeof(temp_buffer), "%s: N/A  ", current_exam->exam_name);
                                } else {
                                    snprintf(temp_buffer, sizeof(temp_buffer), "%s: %d  ", current_exam->exam_name,
                                             score);
                                }

                                if (strlen(exam_buffer) > 50) {
                                    break;
                                }

                                strcat(exam_buffer, temp_buffer);
                                current_exam = current_exam->next;
                                exam_index++;
                            }

                            printf("%-49.49s", exam_buffer);

                            if (current_enrollment->course_average == -1) {
                                printf("%-8.8s", "N/A");
                            } else {
                                printf("%-8.2f", current_enrollment->course_average);
                            }

                            if (strcmp(current_enrollment->letter_grade, "--") == 0) {
                                printf("%-8.8s", "N/A");
                            } else {
                                printf("%-8.8s", current_enrollment->letter_grade);
                            }

                            print_gpa(current_student->GPA);

                            if (strcmp(current_enrollment->letter_grade, "--") == 0) {
                                printf("%-8.8s\n", "PENDING");
                            } else if (strcmp(current_enrollment->letter_grade, "FF") == 0) {
                                printf("%-8.8s\n", "FAILED");
                            } else {
                                printf("%-8.8s\n", "PASSED");
                            }
                        }

                        current_student = current_student->next;
                    }

                    if (!header_printed) {
                        printf("\n!ERROR! There are no students enrolled in the course with ID '%s', or no one has a letter "
                               "grade of "
                               "'%s'\n\n",
                               selected_course_id, letter_grade);
                    }

                    printf("\n\n");
                    break;
                }

                /*
                 * Find Students by Course Average Range in a Course Prompts the user for a course ID, followed by a
                 * maximum and minimum average value. Filters and prints a transcript table of students whose course
                 * average  strictly falls within the specified [min, max] boundaries.
                 */
                case 4: {

                    print_all_courses(course_head);

                    char selected_course_id[10];

                    get_safe_string(3, selected_course_id, sizeof(selected_course_id),
                                    "\nEnter the course ID you want to find students by course average range (or type "
                                    "'exit' to cancel): ");

                    for (int i = 0; selected_course_id[i] != '\0'; i++) {
                        selected_course_id[i] = toupper((unsigned char)selected_course_id[i]);
                    }

                    if (strcmp(selected_course_id, "EXIT") == 0) {
                        printf("\nOperation cancelled. Returning to the previous menu...\n\n");
                        break;
                    }

                    course *current_course = course_head;

                    while (current_course != NULL) {

                        if (strcmp(current_course->course_id, selected_course_id) == 0) {
                            break;
                        }

                        current_course = current_course->next;
                    }

                    if (current_course == NULL) {
                        printf("\n!ERROR! There is no course with ID '%s' in the list", selected_course_id);
                        break;
                    }

                    /*
                     * Prompt for the upper and lower bounds of the search range. A value of -1.0 acts as a safety
                     * trigger to cancel and return to the menu.
                     */

                    float max_course_avg;
                    int attempts = 0;

                    while (1) {

                        attempts++;

                        if (attempts > 3) {
                            printf("\n!ERROR! Maximum attempts reached! Cancelling operation...\n\n");
                            max_course_avg = -1.0;
                            break;
                        }

                        max_course_avg = get_safe_float(-1.0, 100.0, 3,
                                                        "\nEnter the maximum value in the range you want to search (or "
                                                        "enter -1 to return to previous menu): ");

                        if (max_course_avg >= 0.0 || max_course_avg == -1.0) {
                            break;
                        }

                        printf("\n!ERROR! Course average cannot be negative! Enter a value between 0.0 and 100.0, or "
                               "-1 to cancel\n\n");
                    }

                    if (max_course_avg == -1.0) {
                        printf("\nOperation cancelled. Returning to the previous menu...\n\n");
                        break;
                    }

                    float min_course_avg;
                    attempts = 0;

                    while (1) {

                        attempts++;

                        if (attempts > 3) {
                            printf("\n!ERROR! Maximum attempts reached! Cancelling operation...\n\n");
                            max_course_avg = -1.0;
                            break;
                        }

                        min_course_avg = get_safe_float(
                            -1.0, max_course_avg, 3,
                            "\nEnter the minimum value in the range you want to search (minimum value must "
                            "be less than maximum!) (or enter -1 to return to previous menu):  ");

                        if (max_course_avg >= 0.0 || max_course_avg == -1.0) {
                            break;
                        }

                        printf("\n!ERROR! Course average cannot be negative! Enter a value between 0.0 and %.2f, or "
                               "-1 to cancel\n\n",
                               max_course_avg);
                    }

                    if (min_course_avg == -1.0) {
                        printf("\nOperation cancelled. Returning to the previous menu...\n\n");
                        break;
                    }

                    student *current_student = student_head;
                    int header_printed = 0;

                    /*
                     * Iterates through the student list. Skip students who have no enrollments  or who are not
                     * registered for the target course.
                     */
                    while (current_student != NULL) {

                        if (current_student->records == NULL) {
                            current_student = current_student->next;
                            continue;
                        }

                        enrollment *current_enrollment = current_student->records;

                        while (current_enrollment != NULL) {

                            if (strcmp(current_enrollment->course->course_id, current_course->course_id) == 0) {
                                break;
                            }

                            current_enrollment = current_enrollment->next;
                        }

                        if (current_enrollment == NULL) {
                            current_student = current_student->next;
                            continue;
                        }

                        /*
                         * If the student's calculated course average falls within the requested bounds, trigger the
                         * lazy-header (if not already printed) and display their exam records.
                         */
                        if (current_enrollment->course_average >= min_course_avg &&
                            current_enrollment->course_average <= max_course_avg) {

                            if (!header_printed) {
                                printf("%-15s%-30s%-45s%-8s%-8s%-8s%-8s\n", "STUDENT ID", "STUDENT NAME", "EXAM GRADES",
                                       "AVG", "GRADE", "GPA", "STATUS");
                                printf("-------------------------------------------------------------------------------"
                                       "-----------------------------------------\n");
                                header_printed = 1;
                            }

                            printf("%-15u", current_student->id);
                            printf("%-30.30s", current_student->name);

                            exam_template *current_exam = current_enrollment->course->exams;

                            char exam_buffer[75] = "";
                            char temp_buffer[75] = "";
                            int exam_index = 0;

                            while (current_exam != NULL) {

                                int score = current_enrollment->scores[exam_index];

                                if (score == -1) {
                                    snprintf(temp_buffer, sizeof(temp_buffer), "%s: N/A  ", current_exam->exam_name);
                                } else {
                                    snprintf(temp_buffer, sizeof(temp_buffer), "%s: %d  ", current_exam->exam_name,
                                             score);
                                }

                                if (strlen(exam_buffer) > 50) {
                                    break;
                                }

                                strcat(exam_buffer, temp_buffer);
                                current_exam = current_exam->next;
                                exam_index++;
                            }

                            printf("%-49.49s", exam_buffer);

                            if (current_enrollment->course_average == -1) {
                                printf("%-8.8s", "N/A");
                            } else {
                                printf("%-8.2f", current_enrollment->course_average);
                            }

                            if (strcmp(current_enrollment->letter_grade, "--") == 0) {
                                printf("%-8.8s", "N/A");
                            } else {
                                printf("%-8.8s", current_enrollment->letter_grade);
                            }

                            print_gpa(current_student->GPA);

                            if (strcmp(current_enrollment->letter_grade, "--") == 0) {
                                printf("%-8.8s\n", "PENDING");
                            } else if (strcmp(current_enrollment->letter_grade, "FF") == 0) {
                                printf("%-8.8s\n", "FAILED");
                            } else {
                                printf("%-8.8s\n", "PASSED");
                            }
                        }

                        current_student = current_student->next;
                    }

                    if (!header_printed) {
                        printf("\n!ERROR! There are no students enrolled in the course with ID '%s', or there is no one in the "
                               "range\n\n",
                               selected_course_id);
                    }

                    printf("\n\n");
                    break;
                }

                /*
                 * Find Students by Global GPA Range Prompts the user for a maximum and minimum GPA bound. Includes
                 * local attempt counters to safely handle invalid (negative) inputs  without triggering a hard
                 * application exit. Prints a summary of matching students.
                 */
                case 5: {

                    // Uses a local while-loop to prevent infinite error loops if the user enters a negative decimal.
                    float max_GPA;
                    int attempts = 0;

                    while (1) {

                        attempts++;

                        if (attempts > 3) {
                            printf("\n!ERROR! Maximum attempts reached! Cancelling operation...\n");
                            max_GPA = -1.0;
                            break;
                        }

                        max_GPA = get_safe_float(-1.0, 4.0, 3,
                                                 "\nEnter the maximum GPA in the range you want to search (or "
                                                 "enter -1 to return to previous menu): ");

                        if (max_GPA >= 0.0 || max_GPA == -1.0) {
                            break;
                        }

                        printf("\n!ERROR! GPA cannot be negative! Enter a value between 0.0 and 4.0, or "
                               "-1 to cancel\n\n");
                    }

                    if (max_GPA == -1.0) {
                        printf("\nOperation cancelled. Returning to the previous menu...\n\n");
                        break;
                    }

                    // The upper bound is dynamically restricted by the previously entered max_GPA.
                    float min_GPA;
                    attempts = 0;

                    while (1) {

                        attempts++;

                        if (attempts > 3) {
                            printf("\n!ERROR! Maximum attempts reached! Cancelling operation...\n");
                            max_GPA = -1.0;
                            break;
                        }

                        min_GPA = get_safe_float(-1.0, max_GPA, 3,
                                                 "\nEnter the minimum GPA in the range you want to search (or "
                                                 "enter -1 to return to previous menu): ");

                        if (min_GPA >= 0.0 || min_GPA == -1.0) {
                            break;
                        }

                        printf("\n!ERROR! GPA cannot be negative! Enter a value between 0.0 and %.2f, or "
                               "-1 to cancel\n\n",
                               max_GPA);
                    }

                    if (min_GPA == -1.0) {
                        printf("\nOperation cancelled. Returning to the previous menu...\n\n");
                        break;
                    }

                    student *current_student = student_head;
                    int header_printed = 0;

                    while (current_student != NULL) {

                        /*
                         * If the student's global GPA falls strictly within the requested [min, max] boundaries,
                         * trigger the lazy-header (if not already printed) and display their summary record.
                         */
                        if (current_student->GPA >= min_GPA && current_student->GPA <= max_GPA) {

                            if (!header_printed) {
                                printf("\n\n==========================================================================="
                                       "===="
                                       "=========================================\n");
                                printf("STUDENTS IN GPA RANGE\n");
                                printf("==============================================================================="
                                       "=========================================\n\n\n");
                                printf("%-25s%-50s%-8s\n", "STUDENT ID", "STUDENT NAME", "GPA");
                                printf("-------------------------------------------------------------------------------"
                                       "-----------------------------------------\n");
                                header_printed = 1;
                            }

                            printf("%-25u%-50.50s", current_student->id, current_student->name);
                            print_gpa(current_student->GPA);
                            printf("\n");
                        }

                        current_student = current_student->next;
                    }

                    if (!header_printed) {
                        printf("\n!ERROR! There are no students in the GPA range\n\n");
                    }

                    break;
                }

                /*
                 * Search Students by Name (Partial Match) Prompts the user for a keyword, standardizes it to uppercase,
                 * and uses strstr() to find any student whose name contains the keyword (case-insensitive substring
                 * search).
                 */
                case 6: {

                    char temp_name[50];

                    get_safe_name(3, temp_name, sizeof(temp_name),
                                  "\nEnter the word you want to search (or type 'exit' to "
                                  "return to previous menu): ");

                    for (int i = 0; temp_name[i] != '\0'; i++) {
                        temp_name[i] = toupper((unsigned char)temp_name[i]);
                    }

                    if (strcmp(temp_name, "EXIT") == 0) {
                        printf("\nOperation cancelled. Returning to the previous menu...\n\n");
                        break;
                    }

                    /*
                     * Iterate through the student list and check for substring matches. Uses a lazy-header approach to
                     * only print the table header if at least one match is found.
                     */
                    student *current_student = student_head;
                    int header_printed = 0;

                    while (current_student != NULL) {

                        // strstr() returns a non-NULL pointer if 'temp_name' is a substring of 'current_student->name'.
                        if (strstr(current_student->name, temp_name) != NULL) {

                            if (!header_printed) {
                                printf("\n\n==========================================================================="
                                       "===="
                                       "=========================================\n");
                                printf("FIND STUDENTS BY NAME\n");
                                printf("==============================================================================="
                                       "=========================================\n\n\n");
                                printf("%-25s%-50s%-8s\n", "STUDENT ID", "STUDENT NAME", "GPA");
                                printf("-------------------------------------------------------------------------------"
                                       "-----------------------------------------\n");
                                header_printed = 1;
                            }

                            printf("%-25u%-50.50s", current_student->id, current_student->name);
                            print_gpa(current_student->GPA);
                            printf("\n");
                        }

                        current_student = current_student->next;
                    }

                    if (!header_printed) {
                        printf("\n!ERROR! No students found matching '%s'\n\n", temp_name);
                    }

                    printf("\n\n");
                    break;
                }
                }
            }

            break;
        }
        }
    }
}