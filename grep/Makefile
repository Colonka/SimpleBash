CFLAGS = -Wall -Werror -Wextra -std=c11
CC = gcc

all: s21_grep

s21_grep : ./s21_grep.o
	$(CC) ./s21_grep.o -o ./s21_grep
s21_grep.o: ./s21_grep.h ./s21_grep.c
	$(CC) -c $(CFLAGS) ./s21_grep.c
	
check: clang

cppcheck :
	cppcheck --enable=all --suppress=missingIncludeSystem .

clang:
	cp ../../materials/linters/.clang-format .clang-format
	clang-format -n s21_grep.c
	rm .clang-format

git :
	git add Makefile 
	git add ./*.c *.h
	git commit -m "$m"
	git push origin "$b"
	
clean:
	rm -rf ./*.o
	rm -rf ./s21_grep
	
rebuild:
	@$(MAKE) clean
	@$(MAKE) all