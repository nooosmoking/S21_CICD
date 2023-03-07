all: s21_grep

s21_grep: grep.c
	gcc -Wall -Werror -Wextra $< -o $@
test: s21_grep
	bash test_grep.sh
test_leaks: s21_grep
	bash test_leak_grep.sh
clean:
	rm s21_grep
rebuild: 
	clean all
