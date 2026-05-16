#include "../headers/safe_input.h"
#include "../headers/student_management_system.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Exports the entire system RAM (Students, Courses, and Enrollments)
 * into relational CSV database files for persistent storage.
 */
void save_loadable_file(student *student_head, course *course_head) {

    /*
     * =========================================================================
     * PART 1: SAVE STUDENTS (students.csv)
     * Format: ID,Name,GPA
     * =========================================================================
     */
    FILE *new_file = fopen("students.csv", "w");

    if (new_file == NULL) {
        printf("\n!Error! students.csv couldn't be created\n\n");
        return;
    }

    student *current_student = student_head;

    while (current_student != NULL) {
        fprintf(new_file, "%u,%s,%.2f\n", current_student->id, current_student->name, current_student->GPA);
        current_student = current_student->next;
    }

    fclose(new_file);

    /*
     * =========================================================================
     * PART 2: SAVE COURSES (courses.csv)
     * Format: CourseID,CourseName,Credit,Exam1,Weight1,Exam2,Weight2...
     * =========================================================================
     */
    new_file = fopen("courses.csv", "w"); // FIXED: Changed "course.csv" to "courses.csv" to match load function

    if (new_file == NULL) {
        printf("\n!Error! courses.csv couldn't be created\n\n");
        return;
    }

    course *current_course = course_head;

    while (current_course != NULL) {
        // Write base course details
        fprintf(new_file, "%s,%s,%d", current_course->course_id, current_course->course_name,
                current_course->course_credit);

        // Write dynamically allocated exams for the course
        exam_template *current_exam = current_course->exams;
        while (current_exam != NULL) {
            fprintf(new_file, ",%s,%d", current_exam->exam_name, current_exam->exam_percentage);
            current_exam = current_exam->next;
        }

        fprintf(new_file, "\n");
        current_course = current_course->next;
    }

    fclose(new_file);

    /*
     * =========================================================================
     * PART 3: SAVE ENROLLMENTS (enrollments.csv)
     * Format: StudentID,CourseID,Score1,Score2...
     * Acts as the relational junction table mapping students to their course grades.
     * =========================================================================
     */
    new_file = fopen("enrollments.csv", "w");

    if (new_file == NULL) {
        printf("\n!Error! enrollments.csv couldn't be created\n\n");
        return;
    }

    current_student = student_head;

    while (current_student != NULL) {

        enrollment *current_enrollment = current_student->records;

        while (current_enrollment != NULL) {

            // Write relational IDs
            fprintf(new_file, "%u,%s", current_student->id, current_enrollment->course->course_id);

            // Write dynamically allocated scores
            exam_template *current_exam = current_enrollment->course->exams;
            int exam_index = 0;

            while (current_exam != NULL) {
                fprintf(new_file, ",%d", current_enrollment->scores[exam_index]);
                exam_index++;
                current_exam = current_exam->next;
            }

            fprintf(new_file, "\n");
            current_enrollment = current_enrollment->next;
        }

        current_student = current_student->next;
    }

    fclose(new_file);

    printf("\nSystem database successfully backed up to CSV files!\n");

    student_save_flag = 0;
    course_save_flag = 0;
}