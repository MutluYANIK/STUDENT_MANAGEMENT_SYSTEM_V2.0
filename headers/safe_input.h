#ifndef SAFE_INPUT_H
#define SAFE_INPUT_H

void get_safe_name(int max_attempts, char *target, int max_length, const char *message);
int get_safe_int_between(int min, int max, int max_attempts, const char *message);
unsigned int get_safe_unsigned_int(int max_attempts, const char *message);
void get_safe_string(int max_attempts, char *target, int max_length, const char *message);

#endif