#ifndef STUDENT_MANAGEMENT_SYSTEM_H
#define STUDENT_MANAGEMENT_SYSTEM_H

typedef struct exam_template {
    char exam_name[20];
    int exam_percentage;
    struct exam_template *next;
} exam_template;

typedef struct lecture {
    char lecture_id[10];
    char lecture_name[40];
    int lecture_credit;
    struct exam_template *exams;
    struct lecture *next;
} lecture;

typedef struct enrollment {
    struct lecture *lecture;
    int *scores;
    float course_average;
    char letter_grade[3];
    struct enrollment *next;
} enrollment;

typedef struct student {
    unsigned int id;
    char name[50];
    float GPA;
    struct enrollment *records;
    struct student *next;
} student;

typedef enum { SORT_BY_ID, SORT_BY_GPA, SORT_BY_NAME } sort_mode;

extern sort_mode current_mode;

typedef enum { DIR_NORMAL, DIR_REVERSED } sort_direction;

extern sort_direction current_direction;

int add_student(student **student_head);
int add_lecture(lecture **lecture_head);
int id_check(student *student_head, unsigned int id);
int is_lecture_id_exist(lecture *lecture_head, const char *lecture_id);
int enrollment_check(student *current_student, lecture *target_lecture);
int compare_students(student *s1, student *s2);
void grade_calculator(enrollment *enrollment_record);
void calculate_student_gpa(student *target_student);
float get_letter_grade_value(const char *letter_grade);
int update_lecture(student *student_head, lecture **lecture_head);
void delete_lecture(student *student_head, lecture **lecture_head);
int update_student(student **student_head, lecture *lecture_head);
void sort_student_list(student **student_head);
void swap_student(student *student_1, student *student_2);
void sort_student_list_by_id(student **student_head);
void sort_student_list_by_name(student **student_head);
void sort_student_list_by_gpa(student **student_head);
void print_all_lectures(lecture *lecture_head);
void print_enrolled_lectures(student *current_student);
void print_not_enrolled_lectures(student *current_student, lecture *lecture_head);
lecture *find_lecture(lecture *lecture_head, const char *target_lecture_id);
void delete_student(student **student_head, lecture *lecture_head);
void display_student(student *student_head, lecture *lecture_head);
void print_gpa(float GPA);

#endif