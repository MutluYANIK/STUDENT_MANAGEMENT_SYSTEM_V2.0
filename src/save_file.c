#include "../headers/safe_input.h"
#include "../headers/student_management_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void save_file(student *student_head, course *course_head) {

    if (student_head == NULL) {
        printf("\n!ERROR! The student list is empty\n\n");
        return;
    }

    int loop_flag = 1;

    while (loop_flag) {

        int choice = get_safe_int_between(0, 2, 3,
                                          "\n[0] EXIT"
                                          "\n[1] TEXT FILE (.txt - VIEW ONLY)"
                                          "\n[2] EXCEL TABLE (.csv - LOADABLE)\n"
                                          "\nEnter the operation you want to perform: ");

        switch (choice) {

        case 0:
            printf("\nOperation cancelled. Returning to the previous menu...\n\n");
            loop_flag = 0;
            break;

        case 1: {

            int loop_flag_2 = 1;

            while (loop_flag_2) {

                int mode = get_safe_int_between(0, 3, 3,
                                                "\n[0] EXIT"
                                                "\n[1] FOR A STUDENT"
                                                "\n[2] FOR ALL STUDENTS"
                                                "\n[3] FOR A SPECIFIC COURSE"
                                                "\nEnter the operation you want to perform: ");

                switch (mode) {

                case 0:
                    printf("\nOperation cancelled. Returning to the previous menu...\n\n");
                    loop_flag_2 = 0;
                    break;

                case 1:{

                    save_file_for_a_student(student_head, course_head);

                    break;
                }

                case 2:{

                    save_file_for_all_students(student_head, course_head);

                    break;
                }

                case 3:{

                    save_file_for_a_specific_course(student_head, course_head);

                    break;
                }
                }
            }
        }

        case 2: {

            save_loadable_file(student_head, course_head);

            break;
        }
        }
    }
}