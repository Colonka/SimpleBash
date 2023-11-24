CFLAGS = -Wall -Werror -Wextra -std=c11
CC = gcc

all: s21_grep

s21_grep : ./s21_grep.o
	$(CC) ./s21_grep.o -o ./s21_grep
s21_grep.o: ./s21_grep.h ./s21_grep.c
	$(CC) -c $(CFLAGS) ./s21_grep.c

test:
	sh test_grep.sh

git:
	git add -A
	git commit -m "$m"
	git push origin "$b"

dvi: 
	mkdir -p docs
	doxygen Doxyfile
	mv html latex docs
	open docs/html/index.html

docs:
	open docs/html/index.html

clean:
	rm -rf ./*.o
	rm -rf ./s21_grep
	
rebuild:
	@$(MAKE) clean
	@$(MAKE) all