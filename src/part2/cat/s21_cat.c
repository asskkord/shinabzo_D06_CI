#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *FLAGS[] = {"-b", "-e",       "-n",
                 "-s", "-t",       "--number-nonblank",
                 "-E", "--number", "--squeeze-blank",
                 "-T"};

int availability(char *str);
int is_flag(const char *str);
void error();
void correct_arr(char *flags[], int size, char *true_arr[]);
void print_file(FILE *tmp);
void e_flag(FILE *file, FILE *tmp, int fl);
void n_flag(FILE *file, FILE *tmp, int fl);
void b_flag(FILE *file, FILE *tmp, int fl);
void t_flag(FILE *file, FILE *tmp, int fl);
void s_flag(FILE *file, FILE *tmp);
void calculate(char *flags[], int size, char *path);
void copy(FILE *dest, FILE *tmp);

int main(int argc, char *argv[]) {
  int error_flag = 0;
  char *path = NULL;
  char *cur_flags[argc];
  int path_flag = 0;
  for (int i = 0; i < argc; i++) cur_flags[i] = "0";
  for (int i = 1; i < argc; i++) {
    if (!is_flag(argv[i])) {
      if (!path_flag)
        path = argv[i];
      else
        error_flag = 1;
      path_flag++;
    } else {
      if (availability(argv[i]))
        cur_flags[i] = argv[i];
      else
        error_flag = 1;
    }
  }
  if (path == NULL || error_flag == 1) {
    printf("Error\n");
  } else {
    char *true_arr[5] = {"0", "0", "0", "0", "0"};
    correct_arr(cur_flags, argc, true_arr);
    calculate(true_arr, 5, path);
  }
  return 0;
}

int availability(char *str) {
  int ret = 0;
  for (int i = 0; i < 10; i++) {
    if (strcmp(FLAGS[i], str) == 0) {
      ret = 1;
      break;
    }
  }
  return ret;
}

int is_flag(const char *str) {
  int ret = 0;
  if (str[0] == '-') ret = 1;
  return ret;
}

void error() {
  printf("Error\n");
  exit(1);
}

void correct_arr(char *flags[], int size, char *true_arr[]) {
  flags[0] = "0";
  for (int i = 0; i < size; i++) {
    char *curr_str = flags[i];
    if (strcmp(curr_str, FLAGS[0]) == 0 || strcmp(curr_str, FLAGS[5]) == 0)
      true_arr[3] = "-b";
    else if (strcmp(curr_str, FLAGS[2]) == 0 || strcmp(curr_str, FLAGS[7]) == 0)
      true_arr[2] = "-n";
    else if (strcmp(curr_str, FLAGS[3]) == 0 || strcmp(curr_str, FLAGS[8]) == 0)
      true_arr[0] = "-s";
    else if (strcmp(curr_str, FLAGS[1]) == 0 || strcmp(curr_str, FLAGS[6]) == 0)
      true_arr[4] = "-e";
    else if (strcmp(curr_str, FLAGS[4]) == 0 || strcmp(curr_str, FLAGS[9]) == 0)
      true_arr[1] = "-t";
    if (strcmp(true_arr[3], FLAGS[0]) == 0 ||
        strcmp(true_arr[3], FLAGS[5]) == 0)
      true_arr[2] = "0";
  }
}

void calculate(char *flags[], int size, char *path) {
  FILE *file = fopen(path, "r");
  if (file == NULL) {
    fprintf(stderr, "cat: %s: No such file or directory\n", path);
    exit(1);
  }
  FILE *tmp = fopen("temp.txt", "w+");
  int count = 0;
  for (int i = 0; i < size; i++) {
    if (strcmp(flags[i], "-e") == 0) {
      e_flag(file, tmp, count);
      count++;
    } else if (strcmp(flags[i], "-n") == 0) {
      n_flag(file, tmp, count);
      count++;
    } else if (strcmp(flags[i], "-b") == 0) {
      b_flag(file, tmp, count);
      count++;
    } else if (strcmp(flags[i], "-t") == 0) {
      t_flag(file, tmp, count);
      count++;
    } else if (strcmp(flags[i], "-s") == 0) {
      s_flag(file, tmp);
      count++;
    }
  }
  if (count)
    print_file(tmp);
  else
    print_file(file);
  fclose(tmp);
  fclose(file);
  remove("temp.txt");
}

void print_file(FILE *tmp) {
  rewind(tmp);
  fseek(tmp, 0, SEEK_SET);
  int c = fgetc(tmp);
  while (c != EOF) {
    printf("%c", c);
    c = fgetc(tmp);
  }
}
void e_flag(FILE *file, FILE *tmp, int fl) {
  FILE *baza = file;
  FILE *dest = fopen("dest.txt", "w");
  if (fl) {
    copy(dest, tmp);
    fclose(dest);
    dest = fopen("dest.txt", "r");
    rewind(tmp);
    fseek(tmp, 0, SEEK_SET);
    baza = dest;
  }
  int c = fgetc(baza);
  while (c != EOF) {
    if (c == '\n') {
      fputc('$', tmp);
    }
    fputc(c, tmp);
    c = fgetc(baza);
  }
  fclose(dest);
  remove("dest.txt");
}

void n_flag(FILE *file, FILE *tmp, int fl) {
  int counter = 2;
  FILE *baza = file;
  FILE *dest = fopen("dest.txt", "w");
  if (fl) {
    copy(dest, tmp);
    fclose(dest);
    dest = fopen("dest.txt", "r");
    rewind(tmp);
    fseek(tmp, 0, SEEK_SET);
    baza = dest;
  }
  int c = fgetc(baza);
  fprintf(tmp, "%6.d\t", 1);
  while (c != EOF) {
    fputc(c, tmp);
    if (c == '\n') {
      fprintf(tmp, "%6.d\t", counter);
      counter++;
    }
    c = fgetc(baza);
  }
  fclose(dest);
  remove("dest.txt");
}

void b_flag(FILE *file, FILE *tmp, int fl) {
  int counter = 1;
  FILE *baza = file;
  FILE *dest = fopen("dest.txt", "w");
  if (fl) {
    copy(dest, tmp);
    fclose(dest);
    dest = fopen("dest.txt", "r");
    rewind(tmp);
    fseek(tmp, 0, SEEK_SET);
    baza = dest;
  }
  int c = fgetc(baza);
  int nextch = fgetc(baza);
  if (c != '\n') {
    fprintf(tmp, "%6.d\t", counter);
    counter++;
  }
  while (nextch != EOF) {
    fputc(c, tmp);
    if (c == '\n') {
      if (nextch != '\n') {
        fprintf(tmp, "%6.d\t", counter);
        counter++;
      }
    }
    c = nextch;
    nextch = fgetc(baza);
  }
  fputc(c, tmp);
  fclose(dest);
  remove("dest.txt");
}

void t_flag(FILE *file, FILE *tmp, int fl) {
  FILE *baza = file;
  FILE *dest = fopen("dest.txt", "w");
  if (fl) {
    copy(dest, tmp);
    fclose(dest);
    dest = fopen("dest.txt", "r");
    rewind(tmp);
    fseek(tmp, 0, SEEK_SET);
    baza = dest;
  }
  int c = fgetc(baza);
  while (c != EOF) {
    if (c == '\t') {
      fputc('^', tmp);
      fputc('I', tmp);
    } else {
      fputc(c, tmp);
    }
    c = fgetc(baza);
  }
  fclose(dest);
  remove("dest.txt");
}

void s_flag(FILE *file, FILE *tmp) {
  char prevch = '0';
  int c = fgetc(file);
  int nextch = fgetc(file);
  // if (c != '\n') {
  //   fputc(c, tmp);
  // }
  while (nextch != EOF) {
    if (!(c == '\n' && prevch == '\n' && nextch == '\n')) {
      fputc(c, tmp);
    }
    prevch = c;
    c = nextch;
    nextch = fgetc(file);
  }
  fputc(c, tmp);
}

void copy(FILE *dest, FILE *tmp) {
  rewind(tmp);
  fseek(tmp, 0, SEEK_SET);
  int c = fgetc(tmp);
  while (c != EOF) {
    fputc(c, dest);
    c = fgetc(tmp);
  }
}