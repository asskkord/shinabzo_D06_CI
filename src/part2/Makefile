CFLAGS = -Wall -Werror -Wextra -std=c11

all: s21_cat s21_grep

s21_cat:
	gcc $(CFLAGS) cat/s21_cat.c -o cat/s21_cat

s21_grep: parse.o s21_grep.o
	gcc $(CFLAGS) grep/s21_grep.o grep/parse.o -o grep/s21_grep

parse.o:
	gcc $(CFLAGS) -c grep/parse.c -o grep/parse.o

s21_grep.o:
	gcc $(CFLAGS) -c grep/s21_grep.c -o grep/s21_grep.o

clean:
	rm -rf cat/*.o
	rm -rf grep/*.o
	rm -rf grep/s21_grep
	rm -rf cat/s21_cat

rebuild: clean all