#include "../headers/safe_input.h"
#include "../headers/student_management_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Exports class rosters and targeted course statistics to a .txt file.
 * Requires user to select a specific course ID to target operations.
 */
void save_file_for_a_specific_course(student *student_head, course *course_head) {

    // Prevent operations if course list is empty
    if (course_head == NULL) {
        printf("\n!ERROR! The course list is empty\n\n");
        return;
    }

    char target_course_id[15];
    get_safe_string(3, target_course_id, sizeof(target_course_id),
                    "\nEnter the Course ID you want to save (or type 'exit' to cancel): ");

    if (strcmp(target_course_id, "exit") == 0) {
        printf("\nOperation cancelled. Returning to the previous menu...\n\n");
        return;
    }

    // Locate the specific course in the linked list
    course *selected_course = course_head;
    while (selected_course != NULL) {
        if (strcmp(selected_course->course_id, target_course_id) == 0) {
            break;
        }
        selected_course = selected_course->next;
    }

    if (selected_course == NULL) {
        printf("\n!ERROR! There is no course with ID '%s' in the system\n\n", target_course_id);
        return;
    }

    int loop_flag = 1;

    while (loop_flag) {
        int choice = get_safe_int_between(0, 5, 3,
                                          "\n[0] EXIT"
                                          "\n[1] CLASS ROSTER WITH GRADES"
                                          "\n[2] DETAILED TRANSCRIPTS OF ENROLLED STUDENTS"
                                          "\n[3] PASSED STUDENTS"
                                          "\n[4] FAILED STUDENTS"
                                          "\n[5] STUDENTS WITH PENDING GRADES\n"
                                          "\nEnter the operation you want to perform: ");

        switch (choice) {
        case 0:
            printf("\nOperation cancelled. Returning to the previous menu...\n\n");
            loop_flag = 0;
            break;

        /*
         * =====================================================================
         * OPTION 1: CLASS ROSTER WITH GRADES
         * Exports a single-line summary per enrolled student for the target course.
         * =====================================================================
         */
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

            fprintf(new_file, "========================================================================================"
                              "======================\n");
            fprintf(new_file, "CLASS ROSTER   |   COURSE ID: %-10s   |   COURSE NAME: %-30s   |   CREDIT: %d\n",
                    selected_course->course_id, selected_course->course_name, selected_course->course_credit);
            fprintf(new_file, "========================================================================================"
                              "======================\n\n");

            fprintf(new_file, "%-15s%-30s%-40s%-8s%-8s%-8s\n", "STUDENT ID", "STUDENT NAME", "EXAM GRADES", "AVG",
                    "GRADE", "STATUS");
            fprintf(new_file, "----------------------------------------------------------------------------------------"
                              "----------------------\n");

            student *current_student = student_head;
            int enrolled_count = 0;

            while (current_student != NULL) {
                enrollment *current_enrollment = current_student->records;

                // Search student's records for the target course
                while (current_enrollment != NULL) {
                    if (strcmp(current_enrollment->course->course_id, selected_course->course_id) == 0) {

                        enrolled_count++;
                        char exam_buffer[75] = "";
                        char temp_buffer[75] = "";

                        exam_template *current_exam = selected_course->exams;
                        int exam_index = 0;

                        // Safely format exam scores
                        while (current_exam != NULL) {
                            int score = current_enrollment->scores[exam_index];

                            if (score == -1) {
                                snprintf(temp_buffer, sizeof(temp_buffer), "%s: N/A  ", current_exam->exam_name);
                            } else {
                                snprintf(temp_buffer, sizeof(temp_buffer), "%s: %d  ", current_exam->exam_name, score);
                            }

                            if (strlen(exam_buffer) > 45)
                                break;

                            strcat(exam_buffer, temp_buffer);
                            current_exam = current_exam->next;
                            exam_index++;
                        }

                        fprintf(new_file, "%-15u%-30.30s%-40.40s", current_student->id, current_student->name,
                                exam_buffer);

                        if (current_enrollment->course_average == -1) {
                            fprintf(new_file, "%-8s", "N/A");
                        } else {
                            fprintf(new_file, "%-8.2f", current_enrollment->course_average);
                        }

                        fprintf(new_file, "%-8s", current_enrollment->letter_grade);

                        if (strcmp(current_enrollment->letter_grade, "FF") == 0) {
                            fprintf(new_file, "%-8s\n", "FAILED");
                        } else if (strcmp(current_enrollment->letter_grade, "--") == 0) {
                            fprintf(new_file, "%-8s\n", "PENDING");
                        } else {
                            fprintf(new_file, "%-8s\n", "PASSED");
                        }

                        break; // Move to next student after finding the target course
                    }
                    current_enrollment = current_enrollment->next;
                }
                current_student = current_student->next;
            }

            if (enrolled_count == 0) {
                fprintf(new_file, "NO STUDENTS ENROLLED IN THIS COURSE.\n");
            }

            printf("\nClass roster successfully saved to '%s'\n\n", file_name);
            fclose(new_file);
            break;
        }

        /*
         * =====================================================================
         * OPTION 2: DETAILED TRANSCRIPTS OF ENROLLED STUDENTS
         * Exports the full academic history (all courses) for students enrolled in the target course.
         * =====================================================================
         */
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

                // First pass: Verify if the student is enrolled in the target course
                int is_enrolled = 0;
                enrollment *check_enrollment = current_student->records;
                while (check_enrollment != NULL) {
                    if (strcmp(check_enrollment->course->course_id, selected_course->course_id) == 0) {
                        is_enrolled = 1;
                        break;
                    }
                    check_enrollment = check_enrollment->next;
                }

                if (!is_enrolled) {
                    current_student = current_student->next;
                    continue; // Skip student if not in target course
                }

                valid_students = 1;

                // Print the student's header
                fprintf(new_file, "===================================================================================="
                                  "===================\n");
                fprintf(new_file,
                        "STUDENT TRANSCRIPT     |     ID: %u     |     NAME: %s     |     GPA: ", current_student->id,
                        current_student->name);

                if (current_student->GPA == -1.0) {
                    fprintf(new_file, "N/A\n");
                } else {
                    fprintf(new_file, "%.2f\n", current_student->GPA);
                }
                fprintf(new_file, "===================================================================================="
                                  "===================\n\n");
                fprintf(new_file, "%-14s%-25s%-10s%-40s%-8s%-8s%-8s\n", "COURSE ID", "COURSE NAME", "CREDIT",
                        "EXAM GRADES", "AVG", "GRADE", "STATUS");
                fprintf(new_file, "------------------------------------------------------------------------------------"
                                  "-------------------\n");

                // Second pass: Print ALL of the student's courses (Detailed Transcript)
                enrollment *current_enrollment = current_student->records;
                while (current_enrollment != NULL) {
                    char exam_buffer[75] = "";
                    char temp_buffer[75] = "";

                    fprintf(new_file, "%-14.14s", current_enrollment->course->course_id);
                    fprintf(new_file, "%-25.25s", current_enrollment->course->course_name);
                    fprintf(new_file, "%-10d", current_enrollment->course->course_credit);

                    exam_template *current_exam = current_enrollment->course->exams;
                    int exam_index = 0;

                    while (current_exam != NULL) {
                        int score = current_enrollment->scores[exam_index];
                        if (score == -1) {
                            snprintf(temp_buffer, sizeof(temp_buffer), "%s: N/A  ", current_exam->exam_name);
                        } else {
                            snprintf(temp_buffer, sizeof(temp_buffer), "%s: %d  ", current_exam->exam_name, score);
                        }
                        if (strlen(exam_buffer) > 40)
                            break;
                        strcat(exam_buffer, temp_buffer);
                        current_exam = current_exam->next;
                        exam_index++;
                    }

                    fprintf(new_file, "%-40.40s", exam_buffer);

                    if (current_enrollment->course_average == -1) {
                        fprintf(new_file, "%-8s", "N/A");
                    } else {
                        fprintf(new_file, "%-8.2f", current_enrollment->course_average);
                    }

                    fprintf(new_file, "%-8s", current_enrollment->letter_grade);

                    if (strcmp(current_enrollment->letter_grade, "FF") == 0) {
                        fprintf(new_file, "%-8s\n", "FAILED");
                    } else if (strcmp(current_enrollment->letter_grade, "--") == 0) {
                        fprintf(new_file, "%-8s\n", "PENDING");
                    } else {
                        fprintf(new_file, "%-8s\n", "PASSED");
                    }
                    current_enrollment = current_enrollment->next;
                }
                fprintf(new_file, "\n\n");
                current_student = current_student->next;
            }

            if (valid_students == 0) {
                fprintf(new_file, "NO STUDENTS ENROLLED IN THIS COURSE.\n");
            }

            printf("\nDetailed transcripts successfully saved to '%s'\n\n", file_name);
            fclose(new_file);
            break;
        }

        /*
         * =====================================================================
         * OPTION 3: PASSED STUDENTS
         * Filters the class roster for students with passing grades.
         * =====================================================================
         */
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

            int header_printed = 0;
            student *current_student = student_head;
            int valid_students = 0;

            while (current_student != NULL) {
                enrollment *current_enrollment = current_student->records;

                while (current_enrollment != NULL) {
                    // Check if target course AND passed
                    if (strcmp(current_enrollment->course->course_id, selected_course->course_id) == 0 &&
                        strcmp(current_enrollment->letter_grade, "--") != 0 &&
                        strcmp(current_enrollment->letter_grade, "FF") != 0) {

                        valid_students = 1;

                        // Print Class Roster Header Once
                        if (!header_printed) {
                            fprintf(new_file, "========================================================================"
                                              "======================================\n");
                            fprintf(new_file, "PASSED STUDENTS   |   COURSE ID: %-10s   |   COURSE NAME: %-30s\n",
                                    selected_course->course_id, selected_course->course_name);
                            fprintf(new_file, "========================================================================"
                                              "======================================\n\n");
                            fprintf(new_file, "%-15s%-30s%-40s%-8s%-8s%-8s\n", "STUDENT ID", "STUDENT NAME",
                                    "EXAM GRADES", "AVG", "GRADE", "STATUS");
                            fprintf(new_file, "------------------------------------------------------------------------"
                                              "--------------------------------------\n");
                            header_printed = 1;
                        }

                        char exam_buffer[75] = "";
                        char temp_buffer[75] = "";
                        exam_template *current_exam = selected_course->exams;
                        int exam_index = 0;

                        while (current_exam != NULL) {
                            int score = current_enrollment->scores[exam_index];
                            snprintf(temp_buffer, sizeof(temp_buffer), "%s: %d  ", current_exam->exam_name, score);
                            if (strlen(exam_buffer) > 45)
                                break;
                            strcat(exam_buffer, temp_buffer);
                            current_exam = current_exam->next;
                            exam_index++;
                        }

                        fprintf(new_file, "%-15u%-30.30s%-40.40s%-8.2f%-8.8s%-8s\n", current_student->id,
                                current_student->name, exam_buffer, current_enrollment->course_average,
                                current_enrollment->letter_grade, "PASSED");

                        break;
                    }
                    current_enrollment = current_enrollment->next;
                }
                current_student = current_student->next;
            }

            if (valid_students == 0) {
                fprintf(new_file, "NO PASSED STUDENTS FOUND FOR THIS COURSE.\n");
            }

            printf("\nPassed roster successfully saved to '%s'\n\n", file_name);
            fclose(new_file);
            break;
        }

        /*
         * =====================================================================
         * OPTION 4: FAILED STUDENTS
         * Filters the class roster for students with an 'FF' grade.
         * =====================================================================
         */
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

            int header_printed = 0;
            student *current_student = student_head;
            int valid_students = 0;

            while (current_student != NULL) {
                enrollment *current_enrollment = current_student->records;

                while (current_enrollment != NULL) {
                    // Check if target course AND failed
                    if (strcmp(current_enrollment->course->course_id, selected_course->course_id) == 0 &&
                        strcmp(current_enrollment->letter_grade, "FF") == 0) {

                        valid_students = 1;

                        if (!header_printed) {
                            fprintf(new_file, "========================================================================"
                                              "======================================\n");
                            fprintf(new_file, "FAILED STUDENTS   |   COURSE ID: %-10s   |   COURSE NAME: %-30s\n",
                                    selected_course->course_id, selected_course->course_name);
                            fprintf(new_file, "========================================================================"
                                              "======================================\n\n");
                            fprintf(new_file, "%-15s%-30s%-40s%-8s%-8s%-8s\n", "STUDENT ID", "STUDENT NAME",
                                    "EXAM GRADES", "AVG", "GRADE", "STATUS");
                            fprintf(new_file, "------------------------------------------------------------------------"
                                              "--------------------------------------\n");
                            header_printed = 1;
                        }

                        char exam_buffer[75] = "";
                        char temp_buffer[75] = "";
                        exam_template *current_exam = selected_course->exams;
                        int exam_index = 0;

                        while (current_exam != NULL) {
                            int score = current_enrollment->scores[exam_index];
                            snprintf(temp_buffer, sizeof(temp_buffer), "%s: %d  ", current_exam->exam_name, score);
                            if (strlen(exam_buffer) > 45)
                                break;
                            strcat(exam_buffer, temp_buffer);
                            current_exam = current_exam->next;
                            exam_index++;
                        }

                        fprintf(new_file, "%-15u%-30.30s%-40.40s%-8.2f%-8.8s%-8s\n", current_student->id,
                                current_student->name, exam_buffer, current_enrollment->course_average,
                                current_enrollment->letter_grade, "FAILED");

                        break;
                    }
                    current_enrollment = current_enrollment->next;
                }
                current_student = current_student->next;
            }

            if (valid_students == 0) {
                fprintf(new_file, "NO FAILED STUDENTS FOUND FOR THIS COURSE.\n");
            }

            printf("\nFailed roster successfully saved to '%s'\n\n", file_name);
            fclose(new_file);
            break;
        }

        /*
         * =====================================================================
         * OPTION 5: STUDENTS WITH PENDING GRADES
         * Filters the class roster for students with unannounced grades ('--').
         * =====================================================================
         */
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

            int header_printed = 0;
            student *current_student = student_head;
            int valid_students = 0;

            while (current_student != NULL) {
                enrollment *current_enrollment = current_student->records;

                while (current_enrollment != NULL) {
                    // Check if target course AND pending
                    if (strcmp(current_enrollment->course->course_id, selected_course->course_id) == 0 &&
                        strcmp(current_enrollment->letter_grade, "--") == 0) {

                        valid_students = 1;

                        if (!header_printed) {
                            fprintf(new_file, "========================================================================"
                                              "======================================\n");
                            fprintf(new_file, "PENDING GRADES    |   COURSE ID: %-10s   |   COURSE NAME: %-30s\n",
                                    selected_course->course_id, selected_course->course_name);
                            fprintf(new_file, "========================================================================"
                                              "======================================\n\n");
                            fprintf(new_file, "%-15s%-30s%-40s%-8s%-8s%-8s\n", "STUDENT ID", "STUDENT NAME",
                                    "EXAM GRADES", "AVG", "GRADE", "STATUS");
                            fprintf(new_file, "------------------------------------------------------------------------"
                                              "--------------------------------------\n");
                            header_printed = 1;
                        }

                        char exam_buffer[75] = "";
                        char temp_buffer[75] = "";
                        exam_template *current_exam = selected_course->exams;
                        int exam_index = 0;

                        while (current_exam != NULL) {
                            int score = current_enrollment->scores[exam_index];
                            if (score == -1) {
                                snprintf(temp_buffer, sizeof(temp_buffer), "%s: N/A  ", current_exam->exam_name);
                            } else {
                                snprintf(temp_buffer, sizeof(temp_buffer), "%s: %d  ", current_exam->exam_name, score);
                            }
                            if (strlen(exam_buffer) > 45)
                                break;
                            strcat(exam_buffer, temp_buffer);
                            current_exam = current_exam->next;
                            exam_index++;
                        }

                        fprintf(new_file, "%-15u%-30.30s%-40.40s%-8s%-8s%-8s\n", current_student->id,
                                current_student->name, exam_buffer, "N/A", "N/A", "PENDING");

                        break;
                    }
                    current_enrollment = current_enrollment->next;
                }
                current_student = current_student->next;
            }

            if (valid_students == 0) {
                fprintf(new_file, "NO STUDENTS WITH PENDING GRADES FOUND.\n");
            }

            printf("\nPending roster successfully saved to '%s'\n\n", file_name);
            fclose(new_file);
            break;
        }
        }
    }
}