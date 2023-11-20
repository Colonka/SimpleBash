#include "s21_cat.h"

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("Needs to args");
  } else if (argc >= 2) {
    cat_args(argc, argv);
  }
}

void cat_args(int argc, char *argv[]) {
  int err = 0;
  char c;
  flag_info flags = {0, 0, 0, 0, 0, 0};
  for (int i = 1; i < argc; i++) {
    if (*argv[i] == '-') {
      if (cat_parse_args(argv[i], &flags, &c) == 0) {
        fprintf(stderr, "s21_cat: illegal option -- %c\n", c);
        err = 1;
      }
    }
  }
  if (err == 0) {
    if ((flags.b_flag || flags.e_flag || flags.n_flag + flags.s_flag ||
         flags.t_flag || flags.v_flag) == 0) {
      for (int i = 1; i < argc; i++) {
        if (*argv[i] != '-') {
          cat_no_flag(argv[i]);
        }
      }
    } else {
      for (int i = 1; i < argc; i++) {
        if (*argv[i] != '-') {
          cat_show(&flags, argv[i]);
        }
      }
    }
  }
}

int cat_parse_args(char *argv, flag_info *flags, char *err_flag) {
  int err = 1;
  argv++;
  if (*argv == '-') {
    argv++;
    if (strcmp(argv, "number-nonblank") == 0) {
      flags->b_flag = 1;
    } else if (*argv == 'E') {
      flags->e_flag = 1;
    } else if (strcmp(argv, "number") == 0) {
      flags->n_flag = 1;
    } else if (strcmp(argv, "squeeze-blank") == 0) {
      flags->s_flag = 1;
    } else if (*argv == 'T') {
      flags->t_flag = 1;
    } else {
      err = 0;
      *err_flag = *argv;
    }
  }
  for (char *it = argv; *it != 0; it++) {
    switch (*it) {
      case 'b':
        flags->b_flag = 1;
        break;
      case 'e':
        flags->e_flag = 1;
        flags->v_flag = 1;
        break;
      case 'n':
        flags->n_flag = 1;
        break;
      case 's':
        flags->s_flag = 1;
        break;
      case 't':
        flags->t_flag = 1;
        flags->v_flag = 1;
        break;
      case 'v':
        flags->v_flag = 1;
        break;
      default:
        err = 0;
        *err_flag = *argv;
    }
  }
  return err;
}

void cat_show(flag_info *flags, char *filename) {
  FILE *f = fopen(filename, "r");
  char c, cprev = 0, q = 0;
  int count = 0, s_be = 0;
  if (f != NULL) {
    while (!feof(f)) {
      c = fgetc(f);
      if (c != EOF) {
        use_flag(&c, cprev, flags, &count, &s_be, q);
        q = 1;
        if (c != '\n') {
          s_be = 0;
        }
      }
      cprev = c;
    }
    fclose(f);
  } else {
    fprintf(stderr, "s21_cat: %s: No such file or directory\n", filename);
  }
}

void use_flag(char *c, const char cprev, flag_info *flags, int *count,
              int *s_be, int q) {
  if ((flags->s_flag) && (*c == '\n') && (cprev == '\n')) {
    if (*s_be == 0) {
      if (flags->n_flag && flags->b_flag == 0 && cprev == '\n') {
        printf("%6d\t", ++(*count));
      }
      if (flags->e_flag && *c == '\n' && (cprev != '\n' || cprev == '\n') &&
          flags->b_flag == 0) {
        printf("$");
      } else if (flags->e_flag && *c == '\n' && cprev == '\n' &&
                 flags->b_flag) {
        printf("      \t$");
      }
      printf("\n");
    }
    *s_be = 1;
  } else if ((flags->t_flag) && (*c == '\t')) {
    if ((flags->b_flag || flags->n_flag) &&
        ((cprev == '\n') || (*c == '\t' && cprev == '\n'))) {
      printf("%6d\t", ++(*count));
    }
    printf("^I");
  } else if ((flags->e_flag) && (*c == '\n')) {
    if (flags->n_flag && (cprev == '\n' || q == 0) && (*c == '\n') &&
        (flags->b_flag == 0)) {
      printf("%6d\t", ++(*count));
      printf("$\n");
    } else if ((flags->b_flag) && (*c == '\n') && (cprev != '\n')) {
      printf("$\n");
    } else if (flags->b_flag) {
      printf("      \t$\n");
    } else {
      printf("$\n");
    }
  } else if ((flags->v_flag) && (((*c >= 0) && (*c <= 31)) || (*c == 127))) {
    if (flags->n_flag && (cprev == '\n' || q == 0) && flags->b_flag == 0) {
      printf("%6d\t", ++(*count));
    }
    if (flags->b_flag && ((cprev == '\n') || (q == 0)) && (*c != '\n')) {
      printf("%6d\t", ++(*count));
    }
    no_print_symbols(*c);
  } else if ((flags->b_flag) && (*c != '\n') && (*count == 0)) {
    printf("%6d\t%c", ++(*count), *c);
  } else if ((flags->b_flag) && (cprev == '\n') && (*c != '\n')) {
    printf("%6d\t%c", ++(*count), *c);
  } else if ((flags->n_flag) && (*count == 0)) {
    printf("%6d\t%c", ++(*count), *c);
  } else if ((flags->n_flag) && (cprev == '\n') && ((flags->b_flag) == 0)) {
    printf("%6d\t%c", ++(*count), *c);
  } else {
    if (*c != '\n') {
      printf("%c", *c);
    } else {
      printf("\n");
    }
  }
}

void no_print_symbols(int c) {
  if (c == 127) {
    printf("^?");
  } else {
    char const *code[32] = {"^@", "^A", "^B", "^C", "^D",  "^E", "^F", "^G",
                            "^H", "\t", "\n", "^K", "^L",  "^M", "^N", "^O",
                            "^P", "^Q", "^R", "^S", "^T",  "^U", "^V", "^W",
                            "^X", "^Y", "^Z", "^[", "^\\", "^]", "^^", "^_"};
    printf("%s", code[c]);
  }
}

void cat_no_flag(char *argv) {
  FILE *f;
  char c[256];
  f = fopen(argv, "r");
  if (f != NULL) {
    while (!feof(f)) {
      fgets(c, 256, f);
      printf("%s", c);
    }
    fclose(f);
  } else {
    fprintf(stderr, "s21_cat: %s: No such file or directory\n", argv);
  }
}
