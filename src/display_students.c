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
                                          "\n[1] DISPLAY A STUDENT (BY ID)"
                                          "\n[2] DISPLAY ALL STUDENTS"
                                          "\n[3] DISPLAY STUDENTS FOR A COURSE"
                                          "\n[4] DISPLAY CLASS AVERAGE"
                                          "\n[5] DISPLAY AVERAGE GPA"
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
                 * Iterates through the selected student's enrollment records and prints a formatted transcript table.
                 * Displays all courses' IDs, names, exam scores, course averages, grades and statuses
                 */
                case 1: {

                    printf("==========================================================================================="
                           "=============================\n");
                    printf("STUDENT TRANSCRIPT     |     ID: %u     |     NAME: %s     |     GPA: %.2f\n",
                           selected_student->id, selected_student->name, selected_student->GPA);
                    printf("==========================================================================================="
                           "=============================\n");
                    printf("%-14s %-25s %-10s %-40s  %-8s  %-8s  %-8s\n", "COURSE ID", "COURSE NAME", "CREDIT",
                           "          EXAM GRADES", "AVG", "GRADE", "STATUS");
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
                 * Uses a two-pass method to keep the CLI clean. Pass 1: Iterates through the records to check if the
                 * student has any passed courses. If there are none, prints a warning and breaks before printing the
                 * table headers.
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
                 * Uses the two-pass method for CLI safety. Pass 1: Iterates through the records to check if the student
                 * has any failed courses ("FF"). If there are none, prints a warning and breaks without drawing the
                 * table.
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
                                                "\n[1] DISPLAY SUMMARY TABLE"
                                                "\n[2] DISPLAY FULL TRANSCRIPTS"
                                                "\n[3] DISPLAY ONLY PASSED COURSES"
                                                "\n[4] DISPLAY ONLY FAILED COURSES\n"
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

                        printf("======================================================================================="
                               "=================================\n");
                        printf("STUDENT TRANSCRIPT     |     ID: %u     |     NAME: %s     |     GPA: %.2f\n",
                               current_student->id, current_student->name, current_student->GPA);
                        printf("======================================================================================="
                               "=================================\n");
                        printf("%-14s %-25s %-10s %-40s  %-8s  %-8s  %-8s\n", "COURSE ID", "COURSE NAME", "CREDIT",
                               "          EXAM GRADES", "AVG", "GRADE", "STATUS");
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
                 * Iterates through the entire student list and prints detailed transcripts exclusively for passed
                 * courses. Implements strict empty-record handling and two-pass validation to keep the CLI clean.
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
                         * Skips the student if they have enrolled courses but NO passed courses. Prevents printing
                         * empty transcript headers.
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

                            printf("%-8.2f", current_enrollment->course_average);
                            printf("%-8.8s", current_enrollment->letter_grade);
                            printf("%-8.8s\n", "PASSED");

                            current_enrollment = current_enrollment->next;
                        }

                        printf("\n\n");
                        current_student = current_student->next;
                    }

                    break;
                }

                /*
                 * DISPLAY ALL FAILED COURSES: Iterates through all students in the system. Uses two-pass validation to
                 * completely skip students who have no courses or no failed ("FF") courses.
                 */
                case 4: {

                    student *current_student = student_head;

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

                        printf("======================================================================================="
                               "=================================\n");
                        printf("STUDENT TRANSCRIPT     |     ID: %u     |     NAME: %s     |     GPA: %.2f\n",
                               current_student->id, current_student->name, current_student->GPA);
                        printf("======================================================================================="
                               "=================================\n");
                        printf("%-14s %-25s %-10s %-40s  %-8s  %-8s  %-8s\n", "COURSE ID", "COURSE NAME", "CREDIT",
                               "          EXAM GRADES", "AVG", "GRADE", "STATUS");
                        printf("---------------------------------------------------------------------------------------"
                               "---------------------------------\n");

                        current_enrollment = current_student->records;

                        while (current_enrollment != NULL) {

                            // Skips courses that are PASSED or PENDING, stopping only at "FF" grades.
                            while (current_enrollment != NULL &&
                                   (strcmp(current_enrollment->letter_grade, "--") == 0 ||
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

                        printf("\n\n");
                        current_student = current_student->next;
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

            print_all_lectures(lecture_head);

            char selected_course_id[10];

            get_safe_string(3, selected_course_id, sizeof(selected_course_id),
                            "\nEnter the course ID you want to display (or type 'exit' to cancel): ");

            if (strcmp(selected_course_id, "exit") == 0) {
                printf("\nOperation cancelled. Returning to the previous menu...\n\n");
                break;
            }

            for (int i = 0; selected_course_id[i] != '\0'; i++) {
                selected_course_id[i] = toupper((unsigned char)selected_course_id[i]);
            }

            lecture *selected_course = lecture_head;

            while (selected_course != NULL) {

                if (strcmp(selected_course->lecture_id, selected_course_id) == 0) {
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

                int mode = get_safe_int_between(0, 5, 3,
                                                "\n[0] EXIT"
                                                "\n[1] DISPLAY ALL ENROLLED STUDENTS"
                                                "\n[2] DISPLAY ONLY PASSED STUDENTS"
                                                "\n[3] DISPLAY ONLY FAILED STUDENTS"
                                                "\n[4] DISPLAY ONLY PENDING STUDENTS"
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

                            if (strcmp(current_enrollment->lecture->lecture_id, selected_course_id) == 0) {
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

                        exam_template *current_exam = current_enrollment->lecture->exams;

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

                            if (strlen(exam_buffer) > 40) {
                                break;
                            }

                            strcat(exam_buffer, temp_buffer);
                            current_exam = current_exam->next;
                            exam_index++;
                        }

                        printf("%-45.45s", exam_buffer);

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

                        printf("%-8.2f", current_student->GPA);

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

                    break;
                }

                // Filters and prints only the students who have successfully passed the selected course.
                case 2: {

                    student *current_student = student_head;
                    int header_printed = 0;

                    while (current_student != NULL) {

                        enrollment *current_enrollment = current_student->records;

                        while (current_enrollment != NULL) {

                            if (strcmp(current_enrollment->lecture->lecture_id, selected_course_id) == 0) {
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

                        exam_template *current_exam = current_enrollment->lecture->exams;

                        char exam_buffer[75] = "";
                        char temp_buffer[75] = "";
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

                        printf("%-45.45s", exam_buffer);
                        printf("%-8.2f", current_enrollment->course_average);
                        printf("%-8.8s", current_enrollment->letter_grade);
                        printf("%-8.2f", current_student->GPA);
                        printf("%-8.8s\n", "PASSED");

                        current_student = current_student->next;
                    }

                    // Handle the case where no students passed, or the course has zero enrollments.
                    if (!header_printed) {
                        printf("\n!ERROR! There are no students who passed the course '%s'\n\n", selected_course_id);
                    }

                    break;
                }

                // Filters and prints only the students who have failed ("FF") the selected course.
                case 3: {

                    student *current_student = student_head;
                    int header_printed = 0;

                    while (current_student != NULL) {

                        enrollment *current_enrollment = current_student->records;

                        while (current_enrollment != NULL) {

                            if (strcmp(current_enrollment->lecture->lecture_id, selected_course_id) == 0) {
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

                        exam_template *current_exam = current_enrollment->lecture->exams;

                        char exam_buffer[75] = "";
                        char temp_buffer[75] = "";
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

                        printf("%-45.45s", exam_buffer);
                        printf("%-8.2f", current_enrollment->course_average);
                        printf("%-8.8s", current_enrollment->letter_grade);
                        printf("%-8.2f", current_student->GPA);
                        printf("%-8.8s\n", "FAILED");

                        current_student = current_student->next;
                    }

                    // Handle the case where no students failed, or the course has zero enrollments.
                    if (!header_printed) {
                        printf("\n!ERROR! There are no students who failed the course '%s'\n\n", selected_course_id);
                    }

                    break;
                }

                // Filters and prints only the students with pending grades ("--") for the selected course.
                case 4: {

                    student *current_student = student_head;
                    int header_printed = 0;

                    while (current_student != NULL) {

                        enrollment *current_enrollment = current_student->records;

                        while (current_enrollment != NULL) {

                            if (strcmp(current_enrollment->lecture->lecture_id, selected_course_id) == 0) {
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

                        exam_template *current_exam = current_enrollment->lecture->exams;

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

                            if (strlen(exam_buffer) > 40) {
                                break;
                            }

                            strcat(exam_buffer, temp_buffer);
                            current_exam = current_exam->next;
                            exam_index++;
                        }

                        printf("%-45.45s", exam_buffer);
                        printf("%-8.8s", "N/A");
                        printf("%-8.8s", "N/A");
                        printf("%-8.2f", current_student->GPA);
                        printf("%-8.8s\n", "PENDING");

                        current_student = current_student->next;
                    }

                    // Handle the case where no students are pending, or the course has zero enrollments.
                    if (!header_printed) {
                        printf("\n!ERROR! There are no students with pending grades for the course '%s'\n\n",
                               selected_course_id);
                    }

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

                            if (strcmp(current_enrollment->lecture->lecture_id, selected_course_id) == 0) {
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

                        printf("======================================================================================="
                               "=================================\n");
                        printf("STUDENT TRANSCRIPT     |     ID: %u     |     NAME: %s     |     GPA: %.2f\n",
                               current_student->id, current_student->name, current_student->GPA);
                        printf("======================================================================================="
                               "=================================\n");
                        printf("%-14s %-25s %-10s %-40s  %-8s  %-8s  %-8s\n", "COURSE ID", "COURSE NAME", "CREDIT",
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

                int mode = get_safe_int_between(0, 2, 3,
                                                "\n[0] EXIT"
                                                "\n[1] DISPLAY CLASS AVERAGE FOR A COURSE"
                                                "\n[2] DISPLAY CLASS AVERAGE FOR ALL COURSES"
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

                    if (strcmp(selected_course_id, "exit") == 0) {
                        printf("\nOperation cancelled. Returning to the previous menu...\n\n");
                        break;
                    }

                    for (int i = 0; selected_course_id[i] != '\0'; i++) {
                        selected_course_id[i] = toupper((unsigned char)selected_course_id[i]);
                    }

                    lecture *selected_course = lecture_head;

                    while (selected_course != NULL) {

                        if (strcmp(selected_course->lecture_id, selected_course_id) == 0) {
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

                            if (strcmp(current_enrollment->lecture->lecture_id, selected_course_id) == 0 &&
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
                    
                    // Prevent Divide-by-Zero Exception: Check if any valid students were found before division.
                    if (!enrolled_student_found) {
                        printf("\nThere are no students enrolled in this course with ID: '%s' ", selected_course_id);
                        break;
                    }

                    float course_average = total_average / student_counter;

                    printf("======================================================================================="
                           "=================================\n");
                    printf("Course average for '%s': %.2f\n", selected_course_id, course_average);
                    printf("======================================================================================="
                           "=================================\n");

                    break;
                }
                }
            }
        }
        }
    }
}