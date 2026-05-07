#ifndef STUDENT_MANAGEMENT_SYSTEM_H
#define STUDENT_MANAGEMENT_SYSTEM_H

typedef struct exam_template {
    char exam_name[20];
    int exam_percentage;
    struct exam_template *next;
} exam_template;

typedef struct lectures {
    char lecture_id[10];
    char lecture_name[40];
    struct exam_template *exams;
    struct lectures *next;
} lectures;

typedef struct enrollment{
    struct lectures *lecture;
    int *scores;
    struct enrollment *next;
} enrollment;

typedef struct student {
    unsigned int id;
    char name[50];
    struct enrollment *records;
    struct student *next;
} student;


#endif