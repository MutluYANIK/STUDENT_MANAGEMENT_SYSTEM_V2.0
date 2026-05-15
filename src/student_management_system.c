#include "../headers/student_management_system.h"
#include "../headers/safe_input.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

sort_mode current_mode = SORT_BY_ID;
sort_direction current_direction = DIR_NORMAL;

int student_save_flag = 0;
int course_save_flag = 0;

/*

 * Initializes the core data structures (student and course linked lists) and handles the primary
 * application routing. Implements a fail-safe menu system with robust input validation.
 * Includes memory leak prevention mechanisms by executing deep-clean functions before termination,
 * and utilizes dynamic tracking flags to warn the user of any unsaved changes prior to exiting
 * or clearing lists.
 */
int main() {

    student *my_student_list = NULL;
    lecture *my_course_list = NULL;

    int loop_flag = 1;

    while (loop_flag) {

        printf("\n\n================================================================================================="
               "=======================\n");
        printf("                                            STUDENT MANAGEMENT SYSTEM V2.0");
        printf("\n================================================================================================="
               "=======================\n\n");

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

            if (student_save_flag || course_save_flag) {

                int confirm = get_safe_int_between(
                    0, 2, 3,
                    "\n!WARNING! You have unsaved changes! Exiting the program will delete them forever!\n"
                    "\n[0] CANCEL"
                    "\n[1] SAVE & EXIT"
                    "\n[2] EXIT WITHOUT SAVE\n"
                    "\nEnter the operation you want to perform: ");

                switch (confirm) {

                case 0: {
                    printf("\nOperation cancelled. Returning to the main menu...\n\n");
                    return_main_menu = 1;
                    break;
                }

                case 1: {
                    // save fonksiyonu eklenince düzeltilecek.
                    break;
                }

                case 2: {
                    break;
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

        case 1: {

            int loop_flag_2 = 1;

            while (loop_flag_2) {

                printf("\n\n============================================================================================="
                       "===========================\n");
                printf("                                                  STUDENT MANAGEMENT");
                printf("\n============================================================================================="
                       "===========================\n\n");

                int mode = get_safe_int_between(0, 4, 3,
                                                "\n[0] EXIT"
                                                "\n[1] ADD A STUDENT"
                                                "\n[2] UPDATE A STUDENT"
                                                "\n[3] DELETE A STUDENT"
                                                "\n[4] CLEAR THE STUDENT LIST\n"
                                                "\nEnter the operation you want to perform: ");

                switch (mode) {

                case 0:
                    printf("\nOperation cancelled. Returning to the main menu...\n\n");
                    loop_flag_2 = 0;
                    break;

                case 1: {

                    int loop_flag_3 = 1;
                    while (loop_flag_3) {

                        printf("\n\n====================================================================================="
                               "===================================\n");
                        printf("                                                     ADD A STUDENT");
                        printf("\n====================================================================================="
                               "===================================\n\n");

                        loop_flag_3 = add_student(&my_student_list, my_course_list);
                    }

                    break;
                }

                case 2: {

                    int loop_flag_3 = 1;
                    while (loop_flag_3) {

                        loop_flag_3 = update_student(&my_student_list, my_course_list);
                    }

                    break;
                }

                case 3: {

                    int loop_flag_3 = 1;

                    printf("\n\n====================================================================================="
                           "===================================\n");
                    printf("                                                    DELETE A STUDENT");
                    printf("\n====================================================================================="
                           "===================================\n\n");

                    while (loop_flag_3) {

                        loop_flag_3 = delete_student(&my_student_list);
                    }

                    break;
                }

                case 4: {

                    printf("\n\n========================================================================================="
                           "===============================\n");
                    printf("                                                 CLEAR THE STUDENT LIST");
                    printf("\n========================================================================================="
                           "===============================\n\n");

                    if (my_student_list == NULL) {
                        printf("\n!ERROR! Student list is already empty. Returning to the previous menu...\n\n");
                        break;
                    }

                    if (student_save_flag) {

                        int return_previous_menu = 0;

                        int confirm = get_safe_int_between(0, 2, 3,
                                                           "\n!WARNING! You have unsaved changes! Clearing the student "
                                                           "list will delete them forever!\n"
                                                           "\n[0] CANCEL"
                                                           "\n[1] SAVE & CLEAR"
                                                           "\n[2] CLEAR WITHOUT SAVE\n"
                                                           "\nEnter the operation you want to perform: ");

                        switch (confirm) {

                        case 0: {
                            printf("\nOperation cancelled. Returning to the previous menu...\n\n");
                            return_previous_menu = 1;
                            break;
                        }

                        case 1: {
                            // save fonksiyonu eklenince düzeltilecek.
                            break;
                        }

                        case 2: {
                            break;
                        }
                        }

                        if (return_previous_menu) {
                            break;
                        }
                    }

                    clear_the_student_list(&my_student_list);
                    break;
                }
                }
            }

            break;
        }

        case 2: {

            int loop_flag_2 = 1;

            while (loop_flag_2) {

                printf("\n\n============================================================================================="
                       "===========================\n");
                printf("                                                 COURSE MANAGEMENT");
                printf("\n============================================================================================="
                       "===========================\n\n");

                int mode = get_safe_int_between(0, 4, 3,
                                                "\n[0] EXIT"
                                                "\n[1] ADD A COURSE"
                                                "\n[2] UPDATE A COURSE"
                                                "\n[3] DELETE A COURSE"
                                                "\n[4] CLEAR THE COURSE LIST\n"
                                                "\nEnter the operation you want to perform: ");

                switch (mode) {

                case 0:
                    printf("\nOperation cancelled. Returning to the main menu...\n\n");
                    loop_flag_2 = 0;
                    break;

                case 1: {

                    int loop_flag_3 = 1;
                    while (loop_flag_3) {

                        printf("\n\n====================================================================================="
                               "===================================\n");
                        printf("                                                      ADD A COURSE");
                        printf("\n====================================================================================="
                               "===================================\n\n");

                        loop_flag_3 = add_course(&my_course_list);
                    }

                    break;
                }

                case 2: {

                    int loop_flag_3 = 1;
                    while (loop_flag_3) {

                        loop_flag_3 = update_course(&my_student_list, &my_course_list);
                    }

                    break;
                }

                case 3: {

                    printf("\n\n========================================================================================="
                           "===============================\n");
                    printf("                                                     DELETE A COURSE");
                    printf("\n========================================================================================="
                           "===============================\n\n");

                    int loop_flag_3 = 1;
                    while (loop_flag_3) {

                        loop_flag_3 = delete_course(my_student_list, &my_course_list);
                    }

                    break;
                }

                case 4: {

                    printf("\n\n========================================================================================="
                           "===============================\n");
                    printf("                                                  CLEAR THE COURSE LIST");
                    printf("\n========================================================================================="
                           "===============================\n\n");

                    if (my_course_list == NULL) {
                        printf("\n!ERROR! Course list is already empty. Returning to the previous menu...\n\n");
                        break;
                    }

                    if (course_save_flag) {

                        int return_previous_menu = 0;

                        int confirm = get_safe_int_between(0, 2, 3,
                                                           "\n!WARNING! You have unsaved changes! Clearing the course "
                                                           "list will delete them forever!\n"
                                                           "\n[0] CANCEL"
                                                           "\n[1] SAVE & CLEAR"
                                                           "\n[2] CLEAR WITHOUT SAVE\n"
                                                           "\nEnter the operation you want to perform: ");

                        switch (confirm) {

                        case 0: {
                            printf("\nOperation cancelled. Returning to the previous menu...\n\n");
                            return_previous_menu = 1;
                            break;
                        }

                        case 1: {
                            // save fonksiyonu eklenince düzeltilecek.
                            break;
                        }

                        case 2: {
                            break;
                        }
                        }

                        if (return_previous_menu) {
                            break;
                        }
                    }

                    clear_the_course_list(my_student_list, &my_course_list);
                    break;
                }
                }
            }
            break;
        }

        case 3: {
            display_student(my_student_list, my_course_list);
        }

        case 4: {

            int loop_flag_2 = 1;

            while (loop_flag_2) {

                printf("\n\n========================================================================================="
                       "===============================\n");
                printf("                                                    SORT SETTINGS");
                printf("\n========================================================================================="
                       "===============================\n\n");

                int mode = get_safe_int_between(0, 4, 3,
                                                "\n[0] EXIT"
                                                "\n[1] SORT STUDENT LIST BY ID"
                                                "\n[2] SORT STUDENT LIST BY NAME"
                                                "\n[3] SORT STUDENT LIST BY GPA"
                                                "\n[4] SORT STUDENT LIST REVERSE\n"
                                                "\nEnter the operation you want to perform: ");

                switch (mode) {

                case 0:
                    printf("\nOperation cancelled. Returning to the main menu...\n\n");
                    loop_flag_2 = 0;
                    break;

                case 1: {

                    sort_student_list_by_id(&my_student_list);
                    student_save_flag = 1;

                    break;
                }

                case 2: {

                    sort_student_list_by_name(&my_student_list);
                    student_save_flag = 1;

                    break;
                }

                case 3: {

                    sort_student_list_by_gpa(&my_student_list);
                    student_save_flag = 1;

                    break;
                }

                case 4: {

                    reverse_list(&my_student_list);
                    student_save_flag = 1;

                    break;
                }
                }
            }

            break;
        }

        case 5:
            // daha sonra eklenecek.
            break;

        case 6:
            // daha sonra eklenecek.
            break;
        }
    }
}