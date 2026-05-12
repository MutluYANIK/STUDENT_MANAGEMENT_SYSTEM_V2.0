#include "../headers/safe_input.h"
#include "../headers/student_management_system.h"
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int update_lecture(student *student_head, lecture **lecture_head) {

    // Check if the lecture list is empty returning an error message and return to the main menu
    if (*lecture_head == NULL) {
        printf("\n!ERROR! No lectures available to update\n");
        return 0;
    }

    print_all_lectures(lecture_head);

    char temp_lecture_id[10];
    get_safe_string(3, temp_lecture_id, sizeof(temp_lecture_id),
                    "\nEnter the lecture ID you want to update"
                    "(or type 'exit' to return to main menu): ");

    if (strcmp(temp_lecture_id, "exit") == 0) {
        printf("\nOperation cancelled. Returning to main menu...\n\n");
        return 0;
    }

    // Convert the input lecture ID to uppercase for consistency in comparison
    for (int i = 0; temp_lecture_id[i] != '\0'; i++) {
        temp_lecture_id[i] = toupper((unsigned char)temp_lecture_id[i]);
    }

    lecture *target_lecture = *lecture_head;

    /*
     * Traverse the linked list of lectures to find the lecture with the matching ID. If not found, print an error
     * message and return to the main menu.
     */
    while (target_lecture != NULL && strcmp(target_lecture->lecture_id, temp_lecture_id) != 0) {
        target_lecture = target_lecture->next;
    }

    if (target_lecture == NULL) {
        printf("\n!ERROR! No lecture with ID '%s' found in the list!\n\n", temp_lecture_id);
        return 0;
    }

    /*
     * If the lecture is found, present a menu to the user to select which attribute of the lecture they want to update.
     * loop_flag is used to keep the user in the update menu until they choose to exit, allowing multiple updates
     * in one session.
     */
    int loop_flag = 1;

    while (loop_flag) {
        int choice = get_safe_int_between(0, 7, 3,
                                          "\n[0] EXIT\n"
                                          "\n[1] UPDATE LECTURE ID"
                                          "\n[2] UPDATE LECTURE NAME"
                                          "\n[3] UPDATE LECTURE CREDIT"
                                          "\n[4] ADD A NEW EXAM"
                                          "\n[5] REMOV AN EXAM"
                                          "\n[6] UPDATE AN EXAM NAME"
                                          "\n[7] UPDATE EXAM PERCENTAGES\n"
                                          "\nEnter the operation you want to perform: ");

        switch (choice) {

        case 0:
            printf("\nOperation cancelled. Returning to main menu...\n\n");
            loop_flag = 0;
            break;

        /* Takes the new lecture ID input from the user, checks if it already exists in lecture lists prints an
         * error message if not exists updates the lecture ID
         */
        case 1: {

            char new_lecture_id[10];
            get_safe_string(3, new_lecture_id, sizeof(new_lecture_id),
                            "\nEnter the new lecture ID (or type 'exit' to cancel): ");

            if (strcmp(new_lecture_id, "exit") == 0) {
                printf("\nOperation cancelled. Returning to previous menu...\n\n");
                break;
            }

            for (int i = 0; new_lecture_id[i] != '\0'; i++) {
                new_lecture_id[i] = toupper((unsigned char)new_lecture_id[i]);
            }

            if (is_lecture_id_exist(*lecture_head, new_lecture_id)) {
                printf("\n!ERROR! Lecture ID '%s' already exists!\n\n", new_lecture_id);
                break;
            }

            if (*lecture_head == target_lecture) {
                *lecture_head = target_lecture->next;
            } else {
                lecture *previous = *lecture_head;

                while (previous->next != NULL && previous->next != target_lecture) {
                    previous = previous->next;
                }

                if (previous->next == target_lecture) {
                    previous->next = target_lecture->next;
                }
            }

            strcpy(target_lecture->lecture_id, new_lecture_id);
            target_lecture->next = NULL;

            if (*lecture_head == NULL) {

                *lecture_head = target_lecture;

            } else if (strcmp(target_lecture->lecture_id, (*lecture_head)->lecture_id) < 0) {

                target_lecture->next = *lecture_head;
                *lecture_head = target_lecture;

            } else {

                lecture *current_lecture = *lecture_head;

                while (current_lecture->next != NULL &&
                       strcmp(target_lecture->lecture_id, current_lecture->next->lecture_id) > 0) {
                    current_lecture = current_lecture->next;
                }

                target_lecture->next = current_lecture->next;
                current_lecture->next = target_lecture;
            }

            printf("\nLecture ID has been successfully updated.\n\n");
            break;
        }

        // Updates the lecture name with the new name provided by the user
        case 2:
            char temp_lecture_name[40];

            get_safe_string(3, temp_lecture_name, sizeof(temp_lecture_name),
                            "\nEnter the new lecture name (or type 'exit' to cancel): ");

            if (strcmp(temp_lecture_name, "exit") == 0) {
                printf("\nOperation cancelled. Returning to previous menu...\n\n");
                break;
            }

            strcpy(target_lecture->lecture_name, temp_lecture_name);
            printf("\nLecture name has been successfully updated\n\n");

            break;

        /*
         * Updates the lecture credit with the new credit provided by the user and recalculates the GPA of all
         * students enrolled in this lecture
         */
        case 3: {
            int temp_credit = get_safe_int_between(0, INT_MAX, 3, "\nEnter the new lecture credit (or 0 to cancel): ");

            if (temp_credit == 0) {
                printf("\nOperation cancelled. Returning to previous menu...\n\n");
                break;
            }

            target_lecture->lecture_credit = temp_credit;

            student *current_student = student_head;

            while (current_student != NULL) {
                enrollment *current_enrollment = current_student->records;

                while (current_enrollment != NULL) {

                    if (current_enrollment->lecture == target_lecture) {
                        calculate_student_gpa(current_student);
                        break;
                    }
                    current_enrollment = current_enrollment->next;
                }

                current_student = current_student->next;
            }

            printf("\nLecture credit has been succesfully updated and student GPAs recalculated\n\n");

            break;
        }

        /*
         * Gives the user a chance to cancel the operation while taking the new exam name. Converts the input new exam
         * name to uppercase for consistency in comparison. Checks if the new exam name is already exist in the chosen
         * lecture. Allocates memory for the new exam. Links the new exam to the correct position in the lecture. Takes
         * the new exam percentages for the exams while preventing any exam from taking a 0 or negative percentage.
         * Allocates memory for the new exam score and automatically enters -1 for the new exam, updates the lecture
         * grade and GPA for all students who take the lecture
         */
        case 4: {
            char new_exam_name[20];

            get_safe_string(3, new_exam_name, sizeof(new_exam_name),
                            "\nEnter the new exam name (or type 'exit' to cancel): ");

            if (strcmp(new_exam_name, "exit") == 0) {
                printf("\nOperation cancelled. Returning to previous menu...\n\n");
                break;
            }

            for (int i = 0; new_exam_name[i] != '\0'; i++) {
                new_exam_name[i] = toupper((unsigned char)new_exam_name[i]);
            }

            exam_template *temp_exam = target_lecture->exams;

            int exam_count = 0;

            while (temp_exam != NULL) {
                if (strcmp(temp_exam->exam_name, new_exam_name) == 0) {
                    printf("\n!ERROR! An exam with the name '%s' already exists for this lecture!\n\n", new_exam_name);
                    break;
                }
                exam_count++;
                temp_exam = temp_exam->next;
            }

            exam_template *new_exam = (exam_template *)malloc(sizeof(exam_template));

            if (new_exam == NULL) {
                printf("\n!ERROR! Memory allocation failed. Terminating...\n\n");
                break;
            }

            strcpy(new_exam->exam_name, new_exam_name);
            new_exam->exam_percentage = 0;
            new_exam->next = NULL;

            if (target_lecture->exams == NULL) {
                target_lecture->exams = new_exam;
            } else {
                temp_exam = target_lecture->exams;

                while (temp_exam->next != NULL) {
                    temp_exam = temp_exam->next;
                }
                temp_exam->next = new_exam;
            }
            exam_count++;

            temp_exam = target_lecture->exams;

            int remaining_percentage = 100;
            int exams_left = exam_count;
            int max_percentage_per_exam;

            while (temp_exam != NULL) {

                if (exams_left == 1) {
                    printf(
                        "\nThis is the last exam. It will automatically be assigned the remaining percentage of %d%%\n",
                        remaining_percentage);
                    temp_exam->exam_percentage = remaining_percentage;
                    break;
                }

                max_percentage_per_exam = remaining_percentage - (exams_left - 1);

                char prompt_message[150];
                snprintf(prompt_message, sizeof(prompt_message),
                         "\nRemaining percentage: %d%%\nMax allowed percentage for '%s': %d%%\nEnter percentage: ",
                         remaining_percentage, temp_exam->exam_name, max_percentage_per_exam);

                temp_exam->exam_percentage = get_safe_int_between(1, max_percentage_per_exam, 3, prompt_message);

                remaining_percentage -= temp_exam->exam_percentage;
                exams_left--;
                temp_exam = temp_exam->next;
            }

            student *current_student = student_head;

            while (current_student != NULL) {

                enrollment *current_enrollment = current_student->records;

                while (current_enrollment != NULL) {

                    if (current_enrollment->lecture == target_lecture) {
                        int *temp_scores = (int *)realloc(current_enrollment->scores, exam_count * sizeof(int));

                        if (temp_scores == NULL) {
                            printf("\n!ERROR! Memory allocation failed. Terminating...\n\n");
                            break;
                        }

                        current_enrollment->scores = temp_scores;
                        current_enrollment->scores[exam_count - 1] =
                            -1; // Initialize new exam score to -1 (indicating pending grade)

                        grade_calculator(current_enrollment);
                        calculate_student_gpa(current_student);

                        break;
                    }

                    current_enrollment = current_enrollment->next;
                }

                current_student = current_student->next;
            }

            printf("\nNew exam '%s' successfully added and all student records updated!\n\n", new_exam_name);
            break;
        }

        /*
         * Checks if the exam list is already empty prints an error message and breaks the loop. Gives the user
         * a chance to cancel the operation while taking the target exam name. Converts the input target exam name
         * to uppercase for consistency in comparison. Searches for the target exam index in the exam list and counts
         * the exams. If the target exam is not in the list, prints an error message and breaks the loop. Removes the
         * target exam from the list. If the list becomes empty, prints that message on the terminal and clears the
         * scores list for all the students who take the lecture. If it is not empty, takes the new exam percentages for
         * the remaning exams while preventing any exam from taking 0 or negative percentage.
         */
        case 5: {

            if (target_lecture->exams == NULL) {
                printf("\n!ERROR! There are no exams to remove for this lecture!\n\n");
                break;
            }

            exam_template *current_exam = target_lecture->exams;

            while (current_exam != NULL) {

                printf("%s\n", current_exam->exam_name);
                current_exam = current_exam->next;
            }

            char temp_exam_name[20];

            get_safe_string(3, temp_exam_name, sizeof(temp_exam_name),
                            "\nEnter the exam name you want to remove (or type 'exit' to cancel): ");

            if (strcmp(temp_exam_name, "exit") == 0) {
                printf("\nOperation cancelled. Returning to previous menu...\n\n");
                break;
            }

            for (int i = 0; temp_exam_name[i] != '\0'; i++) {
                temp_exam_name[i] = toupper((unsigned char)temp_exam_name[i]);
            }

            int exam_counter = 0;
            int target_index = 0;
            int is_found = 0;
            exam_template *temp_exam = target_lecture->exams;
            exam_template *previous_exam = NULL;

            while (temp_exam != NULL) {
                exam_counter++;

                if (strcmp(temp_exam->exam_name, temp_exam_name) == 0) {
                    is_found = 1;
                    break;
                }

                target_index++;
                previous_exam = temp_exam;
                temp_exam = temp_exam->next;
            }

            if (!is_found) {
                printf("\n!ERROR! Exam '%s' not found in this lecture!\n\n", temp_exam_name);
                break;
            }

            exam_template *exam_to_delete;

            if (previous_exam == NULL) {
                exam_to_delete = target_lecture->exams;
                target_lecture->exams = exam_to_delete->next;
            } else {
                exam_to_delete = previous_exam->next;
                previous_exam->next = exam_to_delete->next;
            }

            free(exam_to_delete);
            int new_exam_count = exam_counter - 1;

            printf("\nExam '%s' successfully removed from the lecture\n", temp_exam_name);

            if (new_exam_count == 0) {
                printf("\nAll exams for this lecture have been removed\n");

                student *current_student = student_head;

                while (current_student != NULL) {

                    enrollment *current_enrollment = current_student->records;

                    while (current_enrollment != NULL) {

                        if (current_enrollment->lecture == target_lecture) {

                            free(current_enrollment->scores);
                            current_enrollment->scores = NULL;

                            grade_calculator(current_enrollment);
                            calculate_student_gpa(current_student);

                            break;
                        }

                        current_enrollment = current_enrollment->next;
                    }

                    current_student = current_student->next;
                }
                break;
            }

            int remaining_percentage = 100;
            int exams_left = new_exam_count;
            int max_percentage_per_exam;

            temp_exam = target_lecture->exams;

            while (temp_exam != NULL) {

                if (exams_left == 1) {
                    printf(
                        "\nThis is the last exam. It will automatically be assigned the remaining percentage of %d%%\n",
                        remaining_percentage);
                    temp_exam->exam_percentage = remaining_percentage;
                    break;
                }

                max_percentage_per_exam = remaining_percentage - (exams_left - 1);

                char prompt_message[150];
                snprintf(prompt_message, sizeof(prompt_message),
                         "\nRemaining percentage: %d%%\nMax allowed percentage for '%s': %d%%\nEnter percentage: ",
                         remaining_percentage, temp_exam->exam_name, max_percentage_per_exam);

                temp_exam->exam_percentage = get_safe_int_between(1, max_percentage_per_exam, 3, prompt_message);

                remaining_percentage -= temp_exam->exam_percentage;
                exams_left--;
                temp_exam = temp_exam->next;
            }

            student *current_student = student_head;

            while (current_student != NULL) {

                enrollment *current_enrollment = current_student->records;

                while (current_enrollment != NULL) {

                    if (current_enrollment->lecture == target_lecture) {

                        for (int i = target_index; i < new_exam_count; i++) {
                            current_enrollment->scores[i] = current_enrollment->scores[i + 1];
                        }

                        current_enrollment->scores =
                            (int *)realloc(current_enrollment->scores, new_exam_count * sizeof(int));

                        grade_calculator(current_enrollment);
                        calculate_student_gpa(current_student);

                        break;
                    }

                    current_enrollment = current_enrollment->next;
                }

                current_student = current_student->next;
            }
            printf("\nExam removed, percentages redistributed, and all student records successfully updated\n\n");
            break;
        }

        /*
         * Gives the user a chance to cancel the operation while taking the target exam name. Converts the input target
         * exam name to uppercase for consistency in comparison. If the target exam is not in the exam list, prints an
         * error message and breaks the loop. Takes the target exam's new name and converts it to
         * uppercase for consistency in the comparison. Checks if the new name is already exists in the chosen lecture's
         * exam list, prints an error message and breaks the loop.
         */
        case 6: {

            exam_template *current_exam = target_lecture->exams;

            while (current_exam != NULL) {

                printf("%s\n", current_exam->exam_name);
                current_exam = current_exam->next;
            }

            char temp_exam_name[20];

            get_safe_string(3, temp_exam_name, sizeof(temp_exam_name),
                            "\nEnter the name of the exam you want to rename (or type 'exit' to cancel): ");

            if (strcmp(temp_exam_name, "exit") == 0) {
                printf("\nOperation cancelled. Returning to previous menu...\n\n");
                break;
            }

            for (int i = 0; temp_exam_name[i] != '\0'; i++) {
                temp_exam_name[i] = toupper((unsigned char)temp_exam_name[i]);
            }

            current_exam = target_lecture->exams;

            while (current_exam != NULL && strcmp(current_exam->exam_name, temp_exam_name) != 0) {
                current_exam = current_exam->next;
            }

            if (current_exam == NULL) {
                printf("\n!ERROR! No exam found with name '%s' in lecture '%s'\n\n", temp_exam_name,
                       target_lecture->lecture_name);
                break;
            }

            get_safe_string(3, temp_exam_name, sizeof(temp_exam_name), "\nEnter the new name: ");

            for (int i = 0; temp_exam_name[i] != '\0'; i++) {
                temp_exam_name[i] = toupper((unsigned char)temp_exam_name[i]);
            }

            exam_template *temp_exam = target_lecture->exams;
            int already_exist = 0;

            while (temp_exam != NULL) {
                if (strcmp(temp_exam->exam_name, temp_exam_name) == 0) {
                    printf("\n!ERROR! An exam with the name '%s' already exists for this lecture!\n\n", temp_exam_name);
                    already_exist = 1;
                    break;
                }
                temp_exam = temp_exam->next;
            }

            if (already_exist) {
                break;
            }

            strcpy(current_exam->exam_name, temp_exam_name);

            printf("\nExam name changed successfully\n\n");
            break;
        }

        /*
         * Checks if the exam list is already empty prints an error message and breaks the loop. Takes confirmation from
         * the user before to changing all the exam percentages. Takes the new exam percentages for the exams while
         * preventing any exam from taking a 0 or negative percentage. Updates the lecture grade and GPA for
         * all students who take the lecture
         */
        case 7: {

            if (target_lecture->exams == NULL) {
                printf("\n!ERROR! There are no exams to update percentages for this lecture\n\n");
                break;
            }

            int confirm = get_safe_int_between(
                0, 1, 3, "\nYou are about to update all exam percentages \nEnter 1 to contiune (or 0 to cancel)");

            if (confirm == 0) {
                printf("\nOperation cancelled. Returning to previous menu...\n\n");
                break;
            }

            exam_template *temp_exam = target_lecture->exams;

            int exam_count = 0;

            while (temp_exam != NULL) {
                exam_count++;
                temp_exam = temp_exam->next;
            }

            int remaining_percentage = 100;
            int exams_left = exam_count;
            int max_percentage_per_exam;

            temp_exam = target_lecture->exams;

            while (temp_exam != NULL) {

                if (exams_left == 1) {
                    printf(
                        "\nThis is the last exam. It will automatically be assigned the remaining percentage of %d%%\n",
                        remaining_percentage);
                    temp_exam->exam_percentage = remaining_percentage;
                    break;
                }

                max_percentage_per_exam = remaining_percentage - (exams_left - 1);

                char prompt_message[150];
                snprintf(prompt_message, sizeof(prompt_message),
                         "\nRemaining percentage: %d%%\nMax allowed percentage for '%s': %d%%\nEnter percentage: ",
                         remaining_percentage, temp_exam->exam_name, max_percentage_per_exam);

                temp_exam->exam_percentage = get_safe_int_between(1, max_percentage_per_exam, 3, prompt_message);

                remaining_percentage -= temp_exam->exam_percentage;
                exams_left--;
                temp_exam = temp_exam->next;
            }

            student *current_student = student_head;

            while (current_student != NULL) {

                enrollment *current_enrollment = current_student->records;

                while (current_enrollment != NULL) {

                    if (current_enrollment->lecture == target_lecture) {
                        grade_calculator(current_enrollment);
                        calculate_student_gpa(current_student);
                        break;
                    }

                    current_enrollment = current_enrollment->next;
                }

                current_student = current_student->next;
            }

            printf("\nAll the exam percentages updated successfully\n\n");
            break;
        }
        }

        if (loop_flag != 0) {
            loop_flag = get_safe_int_between(0, 1, 3, "\nEnter 1 to change something else (or 0 to exit): ");
        }
    }
    return 1;
}