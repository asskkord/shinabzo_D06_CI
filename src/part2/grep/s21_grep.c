#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"

void arr_free(char **arr, int size);
int count_str_infile(char *path);
void calc(char **reg_array, char **files_array, char **flag_array, int reg_size,
          int files_size, int flag_size);
int flag_find(char **arr, char *str, int size);
void print_norm_strings(char *path, int *arr, int str_count, int V_flag,
                        int C_flag, int N_flag, int flag_one);
int arr_sum(int *arr, int size, int V_flag);

int main(int argc, char *argv[]) {
  int flag = 0, reg_size = 0, files_size = 0, flag_size = 0, reg_flag = 0;
  char **reg_array = NULL;
  char **files_array = NULL;
  char **flags_array = NULL;
  char ret_char = flags_parse(argc, argv, &flags_array, &flag_size);
  int S_flag = flag_find(flags_array, "-s", flag_size) == 0 ? 0 : 1;
  if (!S_flag && argc == 1)
    fprintf(stderr,
            "Usage: grep [OPTION]... PATTERNS [FILE]...\nTry 'grep --help' for "
            "more information.\n");
  if (ret_char != '\0') {
    fprintf(stderr,
            "grep: invalid option -- '%c'\nUsage: grep [OPTION]... PATTERNS "
            "[FILE]...\nTry 'grep --help' for more information.\n",
            ret_char);
  } else {
    reg_flag = reg_parse(argc, argv, &reg_array, &reg_size, &flag, S_flag);
    if (!reg_flag) {
      files_parse(argc, argv, &files_array, &files_size, flag);
    }
  }

  calc(reg_array, files_array, flags_array, reg_size, files_size, flag_size);
  if (ret_char == '\0') {
    if (!reg_flag) {
      arr_free(files_array, files_size);
    }
    arr_free(reg_array, reg_size);
  }
  arr_free(flags_array, flag_size);

  return 0;
}

void calc(char **reg_array, char **files_array, char **flag_array, int reg_size,
          int files_size, int flag_size) {  // -e -i -f -v -c -l
  int L_flag = flag_find(flag_array, "-l", flag_size) == 0 ? 0 : 1;
  int S_flag = flag_find(flag_array, "-s", flag_size) == 0 ? 0 : 1;
  int *l_flag_array = (int *)calloc(files_size, sizeof(int));
  for (int i = 0; i < files_size; i++) {
    int *l_match_array = (int *)calloc(files_size, sizeof(int));
    int str_count = count_str_infile(files_array[i]);
    FILE *file = fopen(files_array[i], "r");
    if (file == NULL) {
      if (!S_flag)
        fprintf(stderr, "grep: %s: No such file or directory\n",
                files_array[i]);
      free(l_match_array);
      continue;
    }
    int *norm_string_arr = (int *)calloc(str_count, sizeof(int));
    for (int j = 0; j < str_count; j++) {
      char *fstr = (char *)malloc(0);
      char ch = fgetc(file);
      int index = 0;
      while (ch != '\n' && ch != EOF) {
        fstr = (char *)realloc(fstr, (index + 1) * sizeof(char));
        fstr[index] = ch;
        ch = fgetc(file);
        index++;
      }
      fstr = (char *)realloc(fstr, (index + 1) * sizeof(char));
      fstr[index] = '\0';
      for (int k = 0; k < reg_size; k++) {
        regex_t regex;
        int I_flag =
            flag_find(flag_array, "-i", flag_size) == 0 ? 0 : REG_ICASE;
        int ret = regcomp(&regex, reg_array[k], I_flag);
        ret = regexec(&regex, fstr, 0, NULL, 0);
        if (!ret) norm_string_arr[j] = 1;
        if (L_flag && !ret) l_match_array[k] = 1;
        regfree(&regex);
      }
      free(fstr);
    }
    fclose(file);
    int V_flag = flag_find(flag_array, "-v", flag_size) == 0 ? 0 : 1;
    int C_flag = flag_find(flag_array, "-c", flag_size) == 0 ? 0 : 1;
    int N_flag = flag_find(flag_array, "-n", flag_size) == 0 ? 0 : 1;
    int H_flag = flag_find(flag_array, "-h", flag_size) == 0 ? 0 : 1;
    int flag_one = files_size == 1 ? 1 : 0;
    if (H_flag) flag_one = 1;
    if (!L_flag || (L_flag && C_flag))
      print_norm_strings(files_array[i], norm_string_arr, str_count, V_flag,
                         C_flag, N_flag, flag_one);
    else {
      if (arr_sum(l_match_array, reg_size, V_flag))
        printf("%s\n", files_array[i]);
    }
    free(l_match_array);
    free(norm_string_arr);
  }
  free(l_flag_array);
}

void arr_free(char **arr, int size) {
  for (int i = 0; i < size; i++) {
    free(arr[i]);
  }
  free(arr);
}

int count_str_infile(char *path) {
  char ch;
  int count = 0;
  FILE *file = fopen(path, "r");
  if (file != NULL) {
    while ((ch = fgetc(file)) != EOF) {
      if (ch == '\n') {
        count++;
      }
    }
    count++;
    fclose(file);
  }
  return count;
}

int flag_find(char **arr, char *str, int size) {
  int ret = 0;
  for (int i = 0; i < size; i++) {
    if (strcmp(arr[i], str) == 0) {
      ret = 1;
      break;
    }
  }
  return ret;
}

void print_norm_strings(char *path, int *arr, int str_count, int V_flag,
                        int C_flag, int N_flag, int flag_one) {
  FILE *file = fopen(path, "r");
  int c_flag_count = 0;
  for (int i = 0; i < str_count; i++) {
    int temp = arr[i] + V_flag;
    char ch = fgetc(file);
    if (ch == EOF) continue;
    if (temp % 2 && !flag_one && !C_flag) {
      printf("%s:", path);
    }
    if (temp % 2 && N_flag && !C_flag) {
      printf("%d:", i + 1);
    }
    while (ch != '\n' && ch != EOF) {
      if (temp % 2) {
        if (!C_flag) printf("%c", ch);
      }
      ch = fgetc(file);
    }
    if (temp % 2) {
      if (!C_flag) printf("\n");
    }
    if (temp % 2) {
      if (C_flag) c_flag_count++;
    }
  }
  if (C_flag) {
    if (!flag_one) printf("%s:", path);
    printf("%d\n", c_flag_count);
  }
  fclose(file);
}

int arr_sum(int *arr, int size, int V_flag) {
  int sum = 0;
  for (int i = 0; i < size; i++) {
    if (V_flag && arr[i] == 0)
      sum++;
    else if (!V_flag && arr[i] == 1)
      sum++;
  }
  return sum;
}
