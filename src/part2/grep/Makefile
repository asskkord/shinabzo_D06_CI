CFLAGS = -Wall -Werror -Wextra -std=c11

all: s21_grep

s21_grep: parse.o s21_grep.o
	gcc $(CFLAGS) s21_grep.o parse.o -o s21_grep

parse.o:
	gcc $(CFLAGS) -c parse.c -o parse.o

s21_grep.o:
	gcc $(CFLAGS) -c s21_grep.c -o s21_grep.o

clean:
	rm -rf *.o
	rm -rf s21_grep

rebuild: clean all

test: s21_grep
	./tests1.sh