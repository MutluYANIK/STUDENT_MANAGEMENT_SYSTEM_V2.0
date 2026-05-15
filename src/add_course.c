#include "../headers/safe_input.h"
#include "../headers/student_management_system.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int add_course(lecture **course_head) {
    /*
     * Gives you the chance to exit the function while taking course id input, checks for duplicate
     * course ids to maintain data integrity and prevent confusion in the system.
     */
    char temp_id[10];
    get_safe_string(3, temp_id, sizeof(temp_id), "\nEnter course ID (or type 'exit' to cancel): ");

    for (int i = 0; temp_id[i] != '\0'; i++) {
        temp_id[i] = toupper((unsigned char)temp_id[i]);
    }

    if (strcmp(temp_id, "EXIT") == 0) {
        printf("\nOperation cancelled. Returning to the previous menu...\n\n");
        return 0;
    }

    if (is_course_id_exist(*course_head, temp_id) == 1) {
        printf("\n!ERROR! Course ID already exists!");
        return 1;
    }

    // Allocates memory for a new course and checks for successful allocation to prevent crashes.

    lecture *new_course = (lecture *)malloc(sizeof(lecture));

    lecture *current = *course_head;

    if (new_course == NULL) {
        printf("\n!ERROR! Memory allocation failed!\n\n");
        return 0;
    }

    strcpy(new_course->lecture_id, temp_id);
    /*
     * Takes input for the course name and converts it to uppercase for consistency.
     */
    char temp_course_name[40];
    get_safe_string(3, temp_course_name, sizeof(temp_course_name), "\nEnter course name: ");

    for (int i = 0; temp_course_name[i] != '\0'; i++) {
        temp_course_name[i] = toupper((unsigned char)temp_course_name[i]);
    }

    strcpy(new_course->lecture_name, temp_course_name);

    new_course->lecture_credit = get_positive_int(3, "\nEnter course credit: ");

    int total_percentage = 0;
    new_course->exams = NULL;
    new_course->next = NULL;
    exam_template *exam_tail = NULL;

    printf("\nEnter the exams for %s (The total percentage of all exams must equal 100%%): ",
           new_course->lecture_name);

    /*
     * Allows the user to enter multiple exams for the course, ensuring that the total percanthage of all exams equals
     * 100%. Each exam's name is converted to uppercase for consistency, last exam is added to the end of the linked
     * list of exams for the course, and memory allocation is checked for each new exam to prevent crashes.
     */

    while (total_percentage < 100) {

        int remaining_percentage = 100 - total_percentage;
        char temp_exam_name[20];

        printf("\nRemaining percentage: %d%%\n", remaining_percentage);
        get_safe_string(3, temp_exam_name, sizeof(temp_exam_name), "\nEnter exam name: ");

        for (int i = 0; temp_exam_name[i] != '\0'; i++) {
            temp_exam_name[i] = toupper((unsigned char)temp_exam_name[i]);
        }

        exam_template *current_exam = new_course->exams;
        int is_duplicate = 0;

        while(current_exam != NULL){

            if(strcmp(current_exam->exam_name, temp_exam_name) == 0){
                
                printf("\n!ERROR! An exam already exist in the list with name '%s' enter a different name!\n\n", temp_exam_name);
                is_duplicate = 1;
                break;
            }

            current_exam = current_exam->next;
        }

        if(is_duplicate){
            continue;
        }

        int temp_exam_percentage = get_safe_int_between(1, remaining_percentage, 3, "\nEnter exam percentage: ");

        exam_template *new_exam = (exam_template *)malloc(sizeof(exam_template));

        if (new_exam == NULL) {
            printf("\n!ERROR! Memory allocation failed!\n\n");
            free(new_course);
            return 0;
        }

        strcpy(new_exam->exam_name, temp_exam_name);
        new_exam->exam_percentage = temp_exam_percentage;
        new_exam->next = NULL;

        if (new_course->exams == NULL) {
            new_course->exams = new_exam;
            exam_tail = new_exam;
        } else {
            exam_tail->next = new_exam;
            exam_tail = new_exam;
        }

        total_percentage += temp_exam_percentage;
    }

    printf("\nExam entry complete for %s. Total percentage reached: %d%%\n\n", new_course->lecture_name,
           total_percentage);

    /*
     * Inserts the new course into the linked list of course in sorted order based on course ID to maintain
     * an organized structure.
     */
    if (current == NULL || strcmp(new_course->lecture_id, current->lecture_id) < 0) {
        new_course->next = current;
        *course_head = new_course;
    } else {
        while (current->next != NULL && strcmp(new_course->lecture_id, current->next->lecture_id) > 0) {
            current = current->next;
        }
        new_course->next = current->next;
        current->next = new_course;
    }

    printf("\nCourse '%s' with ID '%s' added successfully!\n\n", new_course->lecture_name, new_course->lecture_id);
    
    course_save_flag = 1;

    return 1;
}