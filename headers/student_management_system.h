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
    struct exam_template *exams;
    struct lecture *next;
} lecture;

typedef struct enrollment{
    struct lecture *lecture;
    int *scores;
    struct enrollment *next;
} enrollment;

typedef struct student {
    unsigned int id;
    char name[50];
    struct enrollment *records;
    struct student *next;
} student;

typedef enum current_mode {
    SORT_BY_ID,
    SORT_BY_GRADE,
    SORT_BY_NAME
} current_mode;

typedef enum current_direction {
    DIR_NORMAL,
    DIR_REVERSED
} current_direction;

int add_student(student **student_head);
int add_lecture(lecture **lecture_head);
int id_check(student *student_head, unsigned int id);
int lecture_id_check(lecture *lecture_head, const char *lecture_id);
int enrollment_check(student *current_student, lecture *target_lecture);
int compare_students(student *s1, student *s2);

#endif