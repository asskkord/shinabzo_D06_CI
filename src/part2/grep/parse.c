#include "parse.h"

int reg_parse(int argc, char *argv[], char ***string, int *arr_size, int *flag,
              int S_flag) {
  int err_flag = 0;

  int first_str_ind = -1;
  for (int i = 1; i < argc; i++) {
    if (*arr_size == 0 && first_str_ind == -1 && argv[i][0] != '-')
      first_str_ind = i;
    if (strcmp(argv[i], "-e") == 0 && i != argc - 1) {
      arr_update(string, arr_size, argv, i + 1);
    } else if (strcmp(argv[i], "-f") == 0 && i != argc - 1) {
      FILE *file = fopen(argv[i + 1], "r");
      if (file == NULL) {
        if (!S_flag)
          fprintf(stderr, "grep: %s: No such file or directory\n", argv[i + 1]);
        err_flag = 1;
        continue;
      }
      while (!feof(file)) {
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
        *string = (char **)realloc(*string, (*arr_size + 1) * sizeof(char *));
        (*string)[*arr_size] = (char *)malloc(strlen(fstr) + 1);
        strcpy((*string)[*arr_size], fstr);
        (*arr_size)++;
        free(fstr);
      }
      fclose(file);

    } else if ((strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "-f") == 0) &&
               i == argc - 1) {
      if (!S_flag)
        fprintf(stderr,
                "grep: option requires an argument -- '%c'\nUsage: grep "
                "[OPTION]... PATTERNS [FILE]...\nTry 'grep --help' for more "
                "information.\n",
                argv[i][1]);
      err_flag = 1;
      break;
    }
  }
  if (*arr_size == 0 && first_str_ind != -1) {
    arr_update(string, arr_size, argv, first_str_ind);
    *flag = 1;
  }
  return err_flag;
}

void files_parse(int argc, char *argv[], char ***files, int *arr_size,
                 int flag) {
  if (flag) {
    int counter = 0;
    for (int i = 1; i < argc; i++) {
      if (argv[i][0] != '-') {
        if (counter > 0) {
          arr_update(files, arr_size, argv, i);
        }
        counter++;
      }
    }
  } else {
    int is_reg = 0;
    for (int i = 1; i < argc; i++) {
      if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "-f") == 0) {
        is_reg = 1;
      } else {
        if (argv[i][0] == '-' || is_reg)
          is_reg = 0;
        else {
          arr_update(files, arr_size, argv, i);
        }
      }
    }
  }
}

char flags_parse(int argc, char *argv[], char ***flags, int *arr_size) {
  char ret_char = '\0';
  for (int i = 1; i < argc; i++) {
    if (is_flag(argv[i])) {
      if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "-f") == 0) continue;
      int exist = 0;
      for (int j = 0; j < *arr_size; j++) {
        if (strcmp(argv[i], (*flags)[j]) == 0) exist = 1;
      }
      if (!exist) arr_update(flags, arr_size, argv, i);
    } else if (strlen(argv[i]) > 2 && argv[i][0] == '-' &&
               strcmp(argv[i - 1], "-e") != 0 &&
               strcmp(argv[i - 1], "-f") != 0) {
      for (int j = 1; j < (int)strlen(argv[i]); j++) {
        char dest[3];
        char char_string[2];
        char_string[0] = argv[i][j];
        char_string[1] = '\0';
        strcpy(dest, "-");
        strcat(dest, char_string);
        if (!is_flag(dest)) {
          ret_char = dest[1];
          break;
        } else {
          if (strcmp(dest, "-e") == 0 || strcmp(dest, "-f") == 0) continue;
          int exist = 0;
          for (int f = 0; f < *arr_size; f++) {
            if (strcmp(dest, (*flags)[f]) == 0) exist = 1;
          }
          if (!exist) {
            *flags = (char **)realloc(*flags, (*arr_size + 1) * sizeof(char *));
            (*flags)[*arr_size] = (char *)malloc(strlen(dest) + 1);
            strcpy((*flags)[*arr_size], dest);
            (*arr_size)++;
          }
        }
      }
    }
  }
  return ret_char;
}

void arr_update(char ***arr, int *size, char *argv[], int argv_ind) {
  *arr = (char **)realloc(*arr, (*size + 1) * sizeof(char *));
  (*arr)[*size] = (char *)malloc(strlen(argv[argv_ind]) + 1);
  strcpy((*arr)[*size], argv[argv_ind]);
  (*size)++;
}

int is_flag(char *str) {
  int ret = 0;
  const char *flags[] = {"-e", "-i", "-v", "-c", "-l",
                         "-n", "-f", "-h", "-o", "-s"};
  for (int i = 0; i < 10; i++) {
    if (strcmp(str, flags[i]) == 0) {
      ret = 1;
      break;
    }
  }
  return ret;
}