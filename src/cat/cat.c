#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct options {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} options;

int parser(int argc, char* argv[], options* opt, int* counter_flags) {
  static struct option long_option[] = {{"number-nonblank", 0, 0, 'b'},
                                        {"number", 0, 0, 'n'},
                                        {"squeeze-blank", 0, 0, 's'},
                                        {"show_ends", 0, 0, 'E'},
                                        {"tubs", 0, 0, 'T'},
                                        {"show_ends_version", 0, 0, 'e'},
                                        {"tubs_version", 0, 0, 't'},
                                        {"tubs_veion", 0, 0, 'v'},
                                        {0, 0, 0, 0}};
  int err = 0;
  char option;
  int options_index;
  while ((option = getopt_long(argc, argv, "+bensvtET", long_option,
                               &options_index)) != (-1)) {
    (*counter_flags)++;
    switch (option) {
      case 'b':
        opt->b = 1;
        break;
      case 'n':
        opt->n = 1;
        break;
      case 's':
        opt->s = 1;
        break;
      case 'E':
        opt->e = 1;
        break;
      case 'T':
        opt->t = 1;
        break;
      case 'e':
        opt->e = 1;
        opt->v = 1;
        break;
      case 'v':
        opt->v = 1;
        break;
      case 't':
        opt->t = 1;
        opt->v = 1;
        break;
      case 0:
        break;
      default:
        fprintf(stderr, "Incorrect option");
        err = 1;
        break;
    }
  }
  return err;
}

int reader(int argc, char* argv[], options opt, int counter_flags) {
  int count_files = 0;
  int frst_check = 1;
  int last = 0;
  for (int i = 0; i + 1 < argc - counter_flags; i++) {
    count_files++;
    FILE* file = fopen(argv[optind + i], "r");
    if (file) {
      int cur;
      int str_count = 0;
      int blank_count = 0;
      int tab = 0;
      while ((cur = fgetc(file)) != EOF) {
        if (opt.s) {
          if ((cur == '\n' && last == '\n') || (frst_check && cur == '\n')) {
            blank_count++;
          } else if (cur != '\n') {
            blank_count = 0;
          }
          if (blank_count > 1) continue;
        }
        if (opt.b) {
          if (frst_check && cur != '\n') {
            printf("%6d\t", ++str_count);
          } else if (cur != '\n' && last == '\n') {
            printf("%6d\t", ++str_count);
          }
        }

        if (opt.n) {
          if (frst_check || last == '\n') {
            printf("%6d\t", ++str_count);
          }
        }
        if (opt.e && cur == '\n') {
          printf("$");
        }
        if (opt.t && cur == '\t') {
          tab = 1;
          if (!opt.v) printf("^I");
        }
        if (opt.v) {
          if (cur >= 32) {
            if (cur < 127)
              printf("%c", cur);
            else if (cur == 127) {
              printf("^");
              printf("?");
            } else {
              putchar('M');
              putchar('-');
              if ((int)cur >= 128 + 32) {
                if (cur < 128 + 127)
                  putchar(cur - 128);
                else {
                  putchar('^');
                  putchar('?');
                }
              } else {
                putchar('^');
                putchar(cur - 128 + 64);
              }
            }
          } else if (cur == '\n')
            putchar('\n');
          else if (cur == '\t' && !opt.t)
            putchar('\t');
          else {
            putchar('^');
            putchar(cur + 64);
          }
        }
        if (blank_count < 2 && tab != 1 && !opt.v) printf("%c", cur);
        last = cur;
        frst_check = 0;
        tab = 0;
      }
    } else {
      printf("cat: %s: No such file or directory\n", argv[optind + i]);
    }
  }
  return count_files;
}

int main(int argc, char* argv[]) {
  int counter_files = 0, counter_flags = 0;
  if (argc > 1) {
    options opt = {0};
    int err = parser(argc, argv, &opt, &counter_flags);
    if (opt.b && opt.n) opt.n = 0;
    if (err != 1) counter_files = reader(argc, argv, opt, counter_flags);
    if (counter_files == 0) {
      printf("No files!");
    }
  } else {
    printf("No arguments!");
  }
  return 0;
}
