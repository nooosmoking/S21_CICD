#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX 100000

typedef struct options {
  int i;
  int e;
  int n;
  int c;
  int l;
  int v;
} options;

int parser(int argc, char* argv[], options* opt, int* counter_flags,
           int* counter_paterns, char* paterns[]) {
  int err = 0;
  char option;
  while ((option = getopt(argc, argv, "ie:nclv")) != (-1)) {
    switch (option) {
      case 'i':
        opt->i = 1;
        break;
      case 'e':
        opt->e = 1;
        paterns[*counter_paterns] = optarg;
        (*counter_paterns)++;
        break;
      case 'n':
        opt->n = 1;
        break;
      case 'c':
        opt->c = 1;
        break;
      case 'l':
        opt->l = 1;
        break;
      case 'v':
        opt->v = 1;
        break;
      case 0:
        break;
      case 1:
        break;
      default:
        fprintf(stderr, "\nIncorrect option %d", option);
        err = 1;
        break;
    }
    (*counter_flags)++;
  }
  return err;
}

int build_pattern(int counter_paterns, char* pattern[], regex_t compiled[],
                  options opt) {
  int flag = REG_NOSUB;
  int result = 0, err = 0;
  if (opt.i) flag |= REG_ICASE;
  for (int i = 0; i < counter_paterns; i++) {
    result = regcomp(&compiled[i], pattern[i], flag);
    if (result) err = 1;
  }
  return err;
}

char read_string(FILE* file, char* string, int* last_string,
                 int* string_counter) {
  char c = getc(file);

  while (c != '\n' && c != EOF) {
    *string++ = c;
    c = getc(file);
  }
  if (*last_string != 1) (*string_counter)++;
  if (c == EOF) *last_string = 1;
  *string++ = '\0';
  return c;
}

int process(options opt, int counter_flags, int counter_paterns, int argc,
            char* argv[], regex_t* compiled) {
  int err = 0;
  FILE* file;
  char string[MAX] = {0};
  int result;
  int counter_files = argc - counter_flags - counter_paterns - 1;
  for (int i = 0; i < counter_files; i++) {
    int count_matches = 0;
    file = fopen(argv[optind + i], "r");
    if (file) {
      int string_counter = 0;
      int last_string = 0;
      int last_read = 0;
      while ((read_string(file, string, &last_string, &string_counter)) !=
                 EOF ||
             (last_string == 1 && !last_read)) {
        if (last_string) last_read++;
        int match_flag = 0;
        for (int n = 0; n < counter_paterns; n++) {
          result = regexec(compiled + n, string, (size_t)0, NULL, 0);
          if ((!result && !opt.v) || (result && opt.v && !last_read && n < 1)) {
            match_flag = 1;
          } else if (!result && opt.v && !last_read) {
            match_flag = 0;
          }
        }

        if (match_flag) count_matches++;
        if (match_flag && !opt.c && !opt.l) {
          if (counter_files > 1) printf("%s:", argv[optind + i]);
          if (opt.n) printf("%d:", string_counter);
          printf("%s\n", string);
        }
      }
      if (opt.c && !opt.l) {
        if (counter_files > 1) printf("%s:", argv[optind + i]);
        printf("%d\n", count_matches);
      }
      if (opt.c && opt.l) {
        if (counter_files > 1) printf("%s:", argv[optind + i]);
        printf("1\n");
      }
      if (opt.l && count_matches) printf("%s\n", argv[optind + i]);
    } else {
      err = 1;
      fprintf(stderr, "No such file or directory\n");
    }
  }
  return err;
}

int main(int argc, char* argv[]) {
  int counter_flags = 0, counter_paterns = 0;

  if (argc > 2) {
    char** patterns = malloc(argc * sizeof(char*));
    regex_t* compiled = (regex_t*)malloc(argc * sizeof(regex_t));
    options opt = {0};
    int err =
        parser(argc, argv, &opt, &counter_flags, &counter_paterns, patterns);

    if (!err) {
      if (!opt.e) {
        patterns[counter_paterns] = argv[optind];
        counter_paterns++;
        optind++;
      }
      err = build_pattern(counter_paterns, patterns, compiled, opt);
      if (!err) {
        process(opt, counter_flags, counter_paterns, argc, argv, compiled);
      }
    }
    free(patterns);
    for (int i = 0; i < counter_paterns; i++) regfree(compiled + i);
    free(compiled);
  } else {
    fprintf(stderr, "Not enough arguments\n");
  }

  return 0;
}