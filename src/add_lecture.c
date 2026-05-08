#include "../headers/safe_input.h"
#include "../headers/student_management_system.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int add_lecture(lecture **lecture_head) {
    /*
     * gives you the chance to exit the function while taking lecture id input, checks for duplicate
     * lecture ids to maintain data integrity and prevent confusion in the system.
     */
    char temp_id[10];
    get_safe_string(3, temp_id, sizeof(temp_id), "\nEnter lecture ID (or type 'exit' to cancel): ");

    if (strcmp(temp_id, "exit") == 0) {
        printf("\nOperation cancelled. Returning to main menu...\n\n");
        return 0;
    }

    if (lecture_id_check(*lecture_head, temp_id) == 1) {
        return 1;
    }

    for (int i = 0; temp_id[i] != '\0'; i++) {
        temp_id[i] = toupper((unsigned char)temp_id[i]);
    }
    // Allocates memory for a new lecture and checks for successful allocation to prevent crashes.

    lecture *new_lecture = (lecture *)malloc(sizeof(lecture));

    lecture *current = *lecture_head;

    if (new_lecture == NULL) {
        printf("\n!ERROR! Memory allocation failed!\n\n");
        return 0;
    }

    strcpy(new_lecture->lecture_id, temp_id);
    /*
     * Takes input for the lecture name and converts it to uppercase for consistency.
     */
    char temp_lecture_name[40];
    get_safe_string(3, temp_lecture_name, sizeof(temp_lecture_name), "\nEnter lecture name: ");

    for (int i = 0; temp_lecture_name[i] != '\0'; i++) {
        temp_lecture_name[i] = toupper((unsigned char)temp_lecture_name[i]);
    }

    strcpy(new_lecture->lecture_name, temp_lecture_name);

    new_lecture->lecture_credit = get_positive_int(3, "\nEnter lecture credit: ");

    int total_percentage = 0;
    new_lecture->exams = NULL;
    new_lecture->next = NULL;
    exam_template *exam_tail = NULL;

    printf("\nEnter the exams for %s (The total percentage of all exams must equal 100%%): ",
           new_lecture->lecture_name);

    /*
     * Allows the user to enter multiple exams for the lecture, ensuring that the total percanthage of all exams equals
     * 100%. Each exam's name is converted to uppercase for consistency, last exam is added to the end of the linked
     * list of exams for the lecture, and memory allocation is checked for each new exam to prevent crashes.
     */

    while (total_percentage < 100) {

        int remaining_percentage = 100 - total_percentage;
        char exam_name[20];
        char temp_exam_name[20];

        printf("\nRemaining percentage: %d%%\n", remaining_percentage);
        get_safe_string(3, temp_exam_name, sizeof(temp_exam_name), "\nEnter exam name: ");

        for (int i = 0; temp_exam_name[i] != '\0'; i++) {
            temp_exam_name[i] = toupper((unsigned char)temp_exam_name[i]);
        }

        strcpy(exam_name, temp_exam_name);

        int exam_percentage = get_safe_int_between(1, remaining_percentage, 3, "\nEnter exam percentage: ");

        exam_template *new_exam = (exam_template *)malloc(sizeof(exam_template));

        if (new_exam == NULL) {
            printf("\n!ERROR! Memory allocation failed!\n\n");
            free(new_lecture);
            return 0;
        }

        strcpy(new_exam->exam_name, exam_name);
        new_exam->exam_percentage = exam_percentage;
        new_exam->next = NULL;

        if (new_lecture->exams == NULL) {
            new_lecture->exams = new_exam;
            exam_tail = new_exam;
        } else {
            exam_tail->next = new_exam;
            exam_tail = new_exam;
        }

        total_percentage += exam_percentage;
    }

    printf("\nExam entry complete for %s. Total percentage reached: %d%%\n\n", new_lecture->lecture_name,
           total_percentage);

    /*
     * Inserts the new lecture into the linked list of lectures in sorted order based on lecture ID to maintain
     * an organized structure.
     */
    if (current == NULL || strcmp(new_lecture->lecture_id, current->lecture_id) < 0) {
        new_lecture->next = current;
        *lecture_head = new_lecture;
    } else {
        while (current->next != NULL && strcmp(new_lecture->lecture_id, current->next->lecture_id) > 0) {
            current = current->next;
        }
        new_lecture->next = current->next;
        current->next = new_lecture;
    }

    printf("\n Lecture '%s' with ID '%s' added successfully!\n\n", new_lecture->lecture_name, new_lecture->lecture_id);

    return 1;
}