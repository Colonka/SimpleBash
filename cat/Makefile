CFLAGS = -Wall -Werror -Wextra -std=c11
CC = gcc

all: s21_cat

s21_cat : ./s21_cat.o
	$(CC) ./s21_cat.o -o ./s21_cat

s21_cat.o: ./s21_cat.h ./s21_cat.c
	$(CC) -c $(CFLAGS) ./s21_cat.c
	
git: clean
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
	rm -rf ./*.o ./s21_cat docs
	
rebuild:
	@$(MAKE) clean
	@$(MAKE) all
