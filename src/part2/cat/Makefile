CFLAGS = -Wall -Werror -Wextra -std=c11

all: s21_cat

s21_cat:
	gcc $(CFLAGS) s21_cat.c -o s21_cat

clean:
	rm -rf *.o
	rm -rf s21_cat

rebuild: clean all

test: s21_cat
	./tests2.sh