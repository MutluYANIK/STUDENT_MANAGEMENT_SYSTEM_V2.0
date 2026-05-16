#include "../headers/safe_input.h"
#include "../headers/student_management_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void load_file(student **student_head, course **course_head) {

    char line[512];
    int line_number;

    /*
     * =========================================================================
     * PART 1: LOAD COURSES (courses.csv)
     * Reads course details and dynamically allocates the exams list.
     * Prevents buffer overflows, invalid credits, and percentage limits (>100).
     * =========================================================================
     */
    FILE *new_file = fopen("courses.csv", "r");

    if (new_file == NULL) {
        printf("\n!WARNING! courses.csv couldn't be opened. No courses loaded\n\n");
    } else {
        line_number = 0;

        // Read line by line until EOF
        while (fgets(line, sizeof(line), new_file) != NULL) {
            line_number++;
            
            // Remove trailing newline character
            line[strcspn(line, "\n")] = 0;

            // Skip completely empty lines to prevent crashes
            if (strlen(line) < 3) continue;

            char *token_id = strtok(line, ",");
            char *token_name = strtok(NULL, ",");
            char *token_credit = strtok(NULL, ",");

            // Validate core course data existence
            if (token_id == NULL || token_name == NULL || token_credit == NULL) {
                printf("\n!WARNING! Line %d in courses.csv is corrupted. Skipping...\n\n", line_number);
                continue;
            }

            int temp_credit = atoi(token_credit);

            // Validate course credit
            if (temp_credit <= 0) {
                printf("\n!WARNING! Line %d: Invalid credit value. Skipping...\n\n", line_number);
                continue;
            }

            // Data is safe. Allocate memory for the new course.
            course *new_course = (course *)malloc(sizeof(course));

            strncpy(new_course->course_id, token_id, sizeof(new_course->course_id) - 1);
            new_course->course_id[sizeof(new_course->course_id) - 1] = '\0';

            strncpy(new_course->course_name, token_name, sizeof(new_course->course_name) - 1);
            new_course->course_name[sizeof(new_course->course_name) - 1] = '\0';

            new_course->course_credit = temp_credit;
            new_course->exams = NULL;
            new_course->next = NULL;

            // --- Load Dynamic Exams for this Course ---
            char *token_exam_name = strtok(NULL, ",");
            exam_template *last_exam = NULL;
            int total_percentage = 0;

            while (token_exam_name != NULL) {
                
                // Skip if exam name is empty
                if (strlen(token_exam_name) == 0) break;

                char *token_percentage = strtok(NULL, ",");

                if (token_percentage == NULL) {
                    printf("\n!WARNING! Line %d: Exam '%s' missing percentage\n\n", line_number, token_exam_name);
                    break;
                }

                int temp_percentage = atoi(token_percentage);

                // Validate individual exam percentage bounds
                if (temp_percentage <= 0 || temp_percentage > 100) {
                    token_exam_name = strtok(NULL, ",");
                    continue;
                }

                // Prevent total exam weights from exceeding 100%
                if (total_percentage + temp_percentage > 100) {
                    printf("\n!WARNING! Line %d: Exam '%s' exceeds 100%% total. Skipping it\n\n", line_number, token_exam_name);
                    token_exam_name = strtok(NULL, ",");
                    continue;
                }

                exam_template *new_exam = (exam_template *)malloc(sizeof(exam_template));

                strncpy(new_exam->exam_name, token_exam_name, sizeof(new_exam->exam_name) - 1);
                new_exam->exam_name[sizeof(new_exam->exam_name) - 1] = '\0';
                
                new_exam->exam_percentage = temp_percentage;
                new_exam->next = NULL;

                total_percentage += temp_percentage;

                // Append to course's exam linked list
                if (new_course->exams == NULL) {
                    new_course->exams = new_exam;
                } else {
                    last_exam->next = new_exam;
                }
                last_exam = new_exam;

                // Move to the next exam in the CSV line
                token_exam_name = strtok(NULL, ",");
            }

            // Append the fully constructed course to the main course list
            if (*course_head == NULL) {
                *course_head = new_course;
            } else {
                course *temp = *course_head;
                while (temp->next != NULL) {
                    temp = temp->next;
                }
                temp->next = new_course;
            }
        }
        fclose(new_file);
        printf("Courses successfully loaded from database.\n");
    }

    /*
     * =========================================================================
     * PART 2: LOAD STUDENTS (students.csv)
     * Reads student info and validates GPA limits (0.0 - 4.0 or -1.0).
     * =========================================================================
     */
    new_file = fopen("students.csv", "r");

    if (new_file == NULL) {
        printf("\n!WARNING! students.csv couldn't be opened. No students loaded\n\n");
    } else {
        line_number = 0;

        while (fgets(line, sizeof(line), new_file) != NULL) {
            line_number++;
            line[strcspn(line, "\n")] = 0;

            if (strlen(line) < 3) continue; // Skip empty lines

            char *token_id = strtok(line, ",");
            char *token_name = strtok(NULL, ",");
            char *token_gpa = strtok(NULL, ",");

            if (token_id == NULL || token_name == NULL || token_gpa == NULL) {
                printf("\n!WARNING! Line %d in students.csv is corrupted. Skipping...\n\n", line_number);
                continue;
            }

            unsigned int temp_id = (unsigned int)atoi(token_id);
            float temp_gpa = atof(token_gpa);

            // Validate GPA (Allow -1.0 for uncalculated, restrict others to 0.0-4.0)
            if (temp_gpa != -1.0 && (temp_gpa < 0.0 || temp_gpa > 4.0)) {
                printf("\n!WARNING! Line %d: Invalid GPA '%.2f'. Skipping student...\n\n", line_number, temp_gpa);
                continue;
            }

            student *new_student = (student *)malloc(sizeof(student));

            new_student->id = temp_id;

            strncpy(new_student->name, token_name, sizeof(new_student->name) - 1);
            new_student->name[sizeof(new_student->name) - 1] = '\0';

            new_student->GPA = temp_gpa;
            new_student->records = NULL;
            new_student->next = NULL;

            // Append to main student list
            if (*student_head == NULL) {
                *student_head = new_student;
            } else {
                student *temp = *student_head;
                while (temp->next != NULL) {
                    temp = temp->next;
                }
                temp->next = new_student;
            }
        }
        fclose(new_file);
        printf("Students successfully loaded from database.\n");
    }

    /*
     * =========================================================================
     * PART 3: LOAD ENROLLMENTS (enrollments.csv)
     * Maps students to courses and loads exam scores.
     * Enforces relational integrity: drops records if course or student is missing.
     * =========================================================================
     */
    new_file = fopen("enrollments.csv", "r");

    if (new_file == NULL) {
        printf("\n!WARNING! enrollments.csv couldn't be opened. No records loaded\n\n");
    } else {
        line_number = 0;

        while (fgets(line, sizeof(line), new_file) != NULL) {
            line_number++;
            line[strcspn(line, "\n")] = 0;

            if (strlen(line) < 3) continue;

            char *token_student_id = strtok(line, ",");
            char *token_course_id = strtok(NULL, ",");

            if (token_student_id == NULL || token_course_id == NULL) continue;

            unsigned int search_student_id = (unsigned int)atoi(token_student_id);

            // Verify if student exists in RAM
            student *found_student = *student_head;
            while (found_student != NULL && found_student->id != search_student_id) {
                found_student = found_student->next;
            }
            if (found_student == NULL) continue; // Orphan record, skip

            // Verify if course exists in RAM
            course *found_course = *course_head;
            while (found_course != NULL && strcmp(found_course->course_id, token_course_id) != 0) {
                found_course = found_course->next;
            }
            if (found_course == NULL) continue; // Orphan record, skip

            // Both exist. Create the enrollment link.
            enrollment *new_enrollment = (enrollment *)malloc(sizeof(enrollment));
            new_enrollment->course = found_course;
            new_enrollment->next = NULL;
            
            // Set default uncalculated values. Grade calculator should be run later.
            new_enrollment->course_average = -1.0;
            strcpy(new_enrollment->letter_grade, "--");

            // Count the number of exams for this specific course
            int exam_count = 0;
            exam_template *counter_exam = found_course->exams;
            while (counter_exam != NULL) {
                exam_count++;
                counter_exam = counter_exam->next;
            }

            // Dynamically allocate the exact memory needed for scores array
            if (exam_count > 0) {
                new_enrollment->scores = (int *)malloc(sizeof(int) * exam_count);
            } else {
                new_enrollment->scores = NULL;
            }

            // Parse and assign scores safely
            exam_template *curr_exam = found_course->exams;
            int idx = 0;
            char *token_score = strtok(NULL, ",");

            while (curr_exam != NULL) {
                if (token_score != NULL) {
                    int score = atoi(token_score);

                    // Validate score bounds. Replace corrupted/invalid data with -1 (N/A)
                    if (score != -1 && (score < 0 || score > 100)) {
                        score = -1;
                    }
                    new_enrollment->scores[idx] = score;
                    token_score = strtok(NULL, ",");
                } else {
                    // Assign -1 if the CSV row ends prematurely
                    new_enrollment->scores[idx] = -1;
                }
                idx++;
                curr_exam = curr_exam->next;
            }

            // Append this enrollment to the student's records list
            if (found_student->records == NULL) {
                found_student->records = new_enrollment;
            } else {
                enrollment *temp_record = found_student->records;
                while (temp_record->next != NULL) {
                    temp_record = temp_record->next;
                }
                temp_record->next = new_enrollment;
            }
        }
        fclose(new_file);
        printf("Enrollments successfully linked and loaded!\n\n");
    }
}