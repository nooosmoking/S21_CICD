all: s21_cat

s21_cat: cat.c
	gcc -Wall -Werror -Wextra $< -o $@
test: s21_cat
	bash test_cat.sh
clean:
	rm s21_cat
rebuild: 
	clean all
