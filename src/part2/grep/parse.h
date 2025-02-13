#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int reg_parse(int argc, char *argv[], char ***string, int *str_size, int *flag,
              int S_flag);
void files_parse(int argc, char *argv[], char ***files, int *arr_size,
                 int flag);
char flags_parse(int argc, char *argv[], char ***flags, int *arr_size);
void arr_update(char ***arr, int *size, char *argv[], int argv_ind);
int is_flag(char *str);

#endif