#include "../headers/safe_input.h"
#include "../headers/student_management_system.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int update_lecture(student *student_head, lecture **lecture_head) {

    // Check if the lecture list is empty returning an error message and return to the main menu
    if (*lecture_head == NULL) {
        printf("No lectures available to update.\n");
        return 0;
    }

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

    lecture *current = *lecture_head;

    /*
     * Traverse the linked list of lectures to find the lecture with the matching ID. If not found, print an error
     * message and return to the main menu.
     */
    while (current != NULL && strcmp(current->lecture_id, temp_lecture_id) != 0) {
        current = current->next;
    }

    if (current == NULL) {
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
                                          "[0] Exit\n"
                                          "[1] Update Lecture ID\n"
                                          "[2] Update Lecture Name\n"
                                          "[3] Update Exam Credit\n"
                                          "[4] Add a new Exam\n"
                                          "[5] Remove an Exam\n"
                                          "[6] Update an Exam Name\n"
                                          "[7] Update an Exam Percentage\n"
                                          "Enter your choice: ");
    }
}
