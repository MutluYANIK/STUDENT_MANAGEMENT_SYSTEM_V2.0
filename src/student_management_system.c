#include "../headers/student_management_system.h"
#include "../headers/safe_input.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

    student *my_student_list = NULL;
    lecture *my_course_list = NULL;

    sort_mode current_mode = SORT_BY_ID;
    sort_direction current_direction = DIR_NORMAL;

    int loop_flag = 1;
    int save_flag = 0;

    while (loop_flag) {

        int choice = get_safe_int_between(0, 6, 3,
                                          "\n[0] EXIT"
                                          "\n[1] STUDENT MANAGEMENT"
                                          "\n[2] COURSE MANAGEMENT"
                                          "\n[3] DISPLAY STUDENTS"
                                          "\n[4] SORT SETTINGS"
                                          "\n[5] LOAD"
                                          "\n[6] SAVE\n"
                                          "\nEnter the operation you want to perform: ");

        switch (choice) {

        case 0: {

            int return_main_menu = 0;
            if (save_flag) {
                int confirm = get_safe_int_between(
                    0, 2, 3,
                    "\n!WARNING! You have unsaved changes! Exiting the program will delete them forever!\n"
                    "\n[0] CANCEL"
                    "\n[1] EXIT WITHOUT SAVEING"
                    "\n[2] SAVE & EXIT\n"
                    "\nEnter the operation you want to perform: ");

                switch (confirm) {

                case 0: {
                    printf("\nOperation cancelled. Returning to the main menu...\n\n");
                    return_main_menu = 1;
                    break;
                }

                case 1: {
                    break;
                }

                case 2: {
                    // save fonksiyonu eklenince düzeltilecek.
                }
                }

                if (return_main_menu) {
                    break;
                }
            }

            clear_the_course_list(my_student_list, &my_course_list);
            clear_the_student_list(&my_student_list);
            loop_flag = 0;
            break;
        }
        }
    }
}