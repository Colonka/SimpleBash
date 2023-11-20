#include "s21_grep.h"

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf(
        "usage: grep [-abcdDEFGHhIiJLlMmnOopqRSsUVvwXxZz] [-A num] [-B num] "
        "[-C[num]]\n\t[-e pattern] [-f file] [--binary-files=value] "
        "[--color=when]\n\t[--context[=num]] [--directories=action] [--label] "
        "[--line-buffered]\n\t[--null] [pattern] [file ...]\n");
  } else {
    Node_pat *head_pat = NULL;
    Node_file *head_file = NULL;
    flags_info flags = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int flag_counter = 0;
    int files_counter = 0;
    int pat_count = 0;
    if (parse_args(&argc, argv, &head_pat, &head_file, &flags, &pat_count,
                   &files_counter)) {
      if ((flag_counter = flags.e + flags.i + flags.v + flags.c + flags.l +
                          flags.n + flags.h + flags.s + flags.f + flags.o) ==
          0) {
        move(&head_file, &head_pat, &pat_count);
        files_counter--;
      }
      grep_func(&flags, &head_file, &head_pat, &files_counter, &flag_counter,
                &pat_count);
    }
    free_pat_file(&head_file, &head_pat);
  }
  return 0;
}

int parse_args(int *argc, char *argv[], Node_pat **head_pat,
               Node_file **head_file, flags_info *flags, int *pat_count,
               int *files_counter) {
  char c;                // pointer to invalid argument
  char *opt = NULL;      // pointer to arg of pattern just after flag
  int ef_was = 0;        // flag that points that there was 'e' or 'f'
  int parce_result = 1;  // flag of error of parcing
  int is_pat_e = 0;      // is pattern after 'e'
  int is_pat_f = 0;      // is pattern after 'f'
  int ef_opt = 0;        // wasn't 'e' or 'f'
  int is_pat = 1;        // wasn't pattern without 'e' or 'f'
  int pat_from_file_error = 1;
  for (int i = 1; i < *argc; i++) {
    if (pat_from_file_error) {
      if (*argv[i] == '-') {
        is_pat_e = 0;
        is_pat_f = 0;
        ef_opt = 0;
        parce_result = parcing(argv[i], &flags, &c, &is_pat_e, &is_pat_f,
                               &ef_opt, &opt, &ef_was);
        if (opt != NULL) {
          if (is_pat_e) {
            push_pattern(opt, &head_pat, &pat_count);
          } else if (is_pat_f) {
            if (push_pattern_from_file(opt, &head_pat, &pat_count) == 0) {
              pat_from_file_error = 0;
              fprintf(stderr, "s21_grep: %s: No such file or directory\n", opt);
              break;
            }
          }
          opt = NULL;
        }
        if (parce_result == 0) {
          fprintf(stderr, "grep: invalid option -- %c\n", c);
        }
      }
      if ((*argv[i] != '-') && (*argv[i - 1] == '-')) {
        if (ef_opt) {
          if (is_pat_e || is_pat_f) {  // read file
            push_file(argv[i], &head_file);
            *files_counter = *files_counter + 1;
            is_pat_e = 0;
            is_pat_f = 0;
          } else if ((is_pat_e == 0) &&
                     (ef_opt == 1)) {  // read pointer to file
            push_pattern(argv[i], &head_pat, &pat_count);
          } else if ((is_pat_f == 0) && (ef_opt == 2)) {
            if (push_pattern_from_file(argv[i], &head_pat, &pat_count) == 0) {
              pat_from_file_error = 0;
              fprintf(stderr, "s21_grep: %s: No such file or directory",
                      argv[i]);
              break;
            }
          }
          ef_opt = 0;
        } else if (is_pat && ef_was == 0) {
          push_pattern(argv[i], &head_pat, &pat_count);
          is_pat = 0;
        } else {
          push_file(argv[i], &head_file);
          *files_counter = *files_counter + 1;
        }
      } else if ((*argv[i - 1] != '-') && (*argv[i] != '-')) {
        push_file(argv[i], &head_file);
        *files_counter = *files_counter + 1;
      }
    }
  }
  return pat_from_file_error;
}

int parcing(char *argv, flags_info **flags, char *err_flag, int *is_pat_e,
            int *is_pat_f, int *ef_opt, char **opt, int *ef_was) {
  argv++;
  int stop = 1;
  int err = 1;
  for (char *it = argv; *it; it++) {
    if (stop) {
      switch (*it) {
        case 'e':
          (*flags)->e = 1;
          stop = 0;
          *ef_opt = 1;
          *ef_was = 1;
          if (*(it + 1)) {
            *opt = (it + 1);
            *is_pat_e = 1;
          }
          break;
        case 'i':
          (*flags)->i = 1;
          break;
        case 'v':
          (*flags)->v = 1;
          break;
        case 'c':
          (*flags)->c = 1;
          break;
        case 'l':
          (*flags)->l = 1;
          break;
        case 'n':
          (*flags)->n = 1;
          break;
        case 'h':
          (*flags)->h = 1;
          break;
        case 's':
          (*flags)->s = 1;
          break;
        case 'f':
          (*flags)->f = 1;
          stop = 0;
          *ef_opt = 2;
          *ef_was = 1;
          if (*(it + 1)) {
            *is_pat_f = 1;
            *opt = (it + 1);
          }
          break;
        case 'o':
          (*flags)->o = 1;
          break;
        default:
          *err_flag = *argv;
          err = 0;
      }
    } else {
      break;
    }
  }
  return err;
}

void grep_func(flags_info *flags, Node_file **head_file, Node_pat **head_pat,
               int *files_counter, int *flag_counter, int *pat_count) {
  Node_file *tmp_file = *head_file;
  Node_pat *tmp_pat = *head_pat;
  Node_pat_o *head_pat_o = NULL;
  int cmp_res = 0;
  char file_str[4096];
  int h_flag = 0;
  int line_counter = 1;
  int bingo_counter = 0;
  int bingo_str = 0;
  int flag_2_files = 1;
  if (flags->h || *files_counter < 2) {
    h_flag = 1;
  }
  if (flags->o) {
    init_reg_o(&head_pat_o, &head_pat, &flags);
  }
  if (tmp_file != NULL) {
    do {
      FILE *f = fopen(tmp_file->file, "r");
      if (f != NULL) {
        while (!(feof(f))) {
          fgets(file_str, 4095, f);
          for (char *i = file_str; *i != '\0'; i++) {
            if (*i == '\n') {
              *i = *(i + 1);
              break;
            }
          }
          if (flags->o) {
            if (regex_func_o(file_str, &head_pat_o, &flags, &(tmp_file->file),
                             &line_counter, &h_flag) != 0) {
              bingo_str++;
            }
          } else if (tmp_pat != NULL) {
            do {
              cmp_res = regex_func(file_str, &(tmp_pat->pat), &flags);
              if (*pat_count == 1) {
                grep_logic(&cmp_res, &flags, &(tmp_file->file), &h_flag,
                           &line_counter, file_str, &flag_counter);
              }
              if (cmp_res == 0) {
                bingo_counter++;
                flag_2_files = 0;
                break;
              }
              tmp_pat = tmp_pat->next_pat;
            } while (tmp_pat != NULL);
            if (*pat_count > 1) {
              grep_logic(&flag_2_files, &flags, &(tmp_file->file), &h_flag,
                         &line_counter, file_str, &flag_counter);
            }
            flag_2_files = 1;
            tmp_pat = *head_pat;
          }
          line_counter++;
        }
        fclose(f);
        if (flags->l) {
          logic_l(&flags, &h_flag, &bingo_counter, &(tmp_file->file),
                  &bingo_str);
        } else if (flags->c) {
          logic_c(&flags, &h_flag, &bingo_counter, &(tmp_file->file),
                  &line_counter, &bingo_str);
        }
        tmp_file = tmp_file->next_file;
        line_counter = 1;
        bingo_counter = 0;
        bingo_str = 0;
      } else if (flags->s == 0) {
        fprintf(stderr, "grep: %s: No such file or directory\n",
                tmp_file->file);
        tmp_file = tmp_file->next_file;
      } else {
        tmp_file = tmp_file->next_file;
      }
    } while (tmp_file != NULL);
    free_pat_o(&head_pat_o);
  }
}

void logic_l(flags_info **flags, int *h_flag, int *bingo_counter, char **file,
             int *bingo_str) {
  if ((*flags)->v && *bingo_counter == 0) {
    printf("%s\n", *file);
  } else if ((*flags)->c) {
    if (*bingo_counter) {
      if (*h_flag) {
        printf("%d\n", 1);
      } else {
        printf("%s:%d\n", *file, 1);
      }
    } else {
      if (*h_flag) {
        printf("%d\n", 0);
      } else {
        printf("%s:%d\n", *file, 0);
      }
    }
  }
  if (*bingo_counter || *bingo_str) {
    printf("%s\n", *file);
  }
}

void logic_c(flags_info **flags, int *h_flag, int *bingo_counter, char **file,
             int *line_counter, int *bingo_str) {
  if ((*flags)->v) {
    if (*h_flag) {
      printf("%d\n", (*line_counter - *bingo_counter - 1));
    } else {
      printf("%s:%d\n", *file, (*line_counter - *bingo_counter - 1));
    }
  } else if ((*flags)->h) {
    printf("%d\n", *bingo_counter);
  } else if ((*flags)->o) {
    if (*h_flag) {
      printf("%d\n", *bingo_str);
    } else {
      printf("%s:%d\n", *file, *bingo_str);
    }
  } else {
    if (*h_flag) {
      printf("%d\n", *bingo_counter);
    } else {
      printf("%s:%d\n", *file, *bingo_counter);
    }
  }
}

void init_reg_o(Node_pat_o **head_pat_o, Node_pat ***head_pat,
                flags_info **flags) {
  if (*(*head_pat) != NULL) {
    Node_pat *tmp = *(*head_pat);
    do {
      Node_pat_o *tmp_o = (Node_pat_o *)malloc(sizeof(Node_pat_o));
      if (*head_pat_o != NULL) {
        Node_pat_o *last = *head_pat_o;
        while (last->next_pat_o != NULL) {
          last = last->next_pat_o;
        }
        if ((*flags)->i) {
          regcomp(&(tmp_o->reg), tmp->pat, REG_ICASE);
        } else {
          regcomp(&(tmp_o->reg), tmp->pat, 0);
        }
        tmp_o->next_pat_o = NULL;
        last->next_pat_o = tmp_o;
      } else {
        if ((*flags)->i) {
          regcomp(&(tmp_o->reg), tmp->pat, REG_ICASE);
        } else {
          regcomp(&(tmp_o->reg), tmp->pat, 0);
        }
        tmp_o->next_pat_o = NULL;
        *head_pat_o = tmp_o;
      }
      tmp = tmp->next_pat;
    } while (tmp != NULL);
  }
}

int regex_func_o(char *str, Node_pat_o **head_pat_o, flags_info **flags,
                 char **file, int *line_counter, int *h_flag) {
  regmatch_t pmatch;
  int bingo = 0;
  Node_pat_o *tmp = *head_pat_o;
  for (char *i = str; *i != '\0';) {
    if (tmp) {
      if ((regexec(&(tmp->reg), i, 1, &pmatch, 0) == 0)) {
        bingo++;
        if ((*flags)->c == 0 && (*flags)->v == 0 && (*flags)->l == 0) {
          if ((*flags)->n) {
            if (*h_flag) {
              printf("%d:", *line_counter);
            } else {
              printf("%s:%d:", *file, *line_counter);
            }
          } else if (*h_flag == 0) {
            printf("%s:", *file);
          }
          for (int j = pmatch.rm_so; j < pmatch.rm_eo; j++) {
            putchar(i[j]);
          }
          printf("\n");
        }
        i = i + pmatch.rm_eo;
      } else {
        tmp = tmp->next_pat_o;
      }
    } else {
      break;
    }
  }
  if ((*flags)->v && bingo == 0) {
    if (*h_flag) {
      printf("%s\n", str);
    } else {
      printf("%s:%s\n", *file, str);
    }
  }
  return bingo;
}

void grep_logic(int *cmp_res, flags_info **flags, char **file, int *h_flag,
                int *line_counter, char *str, int **flag_counter) {
  if ((*flags)->v && (*cmp_res == 1) && (*flags)->c == 0 && (*flags)->l == 0 &&
      (*flags)->o == 0) {
    if ((*flags)->n) {
      if (*h_flag) {
        printf("%d:%s\n", *line_counter, str);
      } else {
        printf("%s:%d:%s\n", *file, *line_counter, str);
      }
    } else {
      if (*h_flag) {
        printf("%s\n", str);
      } else {
        printf("%s:%s\n", *file, str);
      }
    }
  } else if (((*flags)->i) && (*cmp_res == 0) &&
             ((*flags)->v + (*flags)->c + (*flags)->l + (*flags)->o) == 0) {
    if ((*flags)->n) {
      if (*h_flag) {
        printf("%d:%s\n", *line_counter, str);
      } else {
        printf("%s:%d:%s\n", *file, *line_counter, str);
      }
    } else if (*h_flag && (*flags)->c == 0) {
      printf("%s\n", str);
    } else {
      printf("%s:%s\n", *file, str);
    }
  } else if ((*flags)->n && (*cmp_res == 0) && (*flags)->v == 0 &&
             (*flags)->c == 0 && (*flags)->l == 0) {
    if (*h_flag) {
      printf("%d:%s\n", *line_counter, str);
    } else {
      printf("%s:%d:%s\n", *file, *line_counter, str);
    }
  } else if (*cmp_res == 0 &&
             (*(*flag_counter) == 0 || (*flags)->e || (*flags)->f ||
              (*flags)->h || (*flags)->s) &&
             ((*flags)->l == 0 && (*flags)->v == 0 && (*flags)->c == 0)) {
    if (*h_flag && (*flags)->v == 0) {
      printf("%s\n", str);
    } else if (*h_flag == 0) {
      printf("%s:%s\n", *file, str);
    }
  }
}

int regex_func(char *str, char **pattern, flags_info **flags) {
  regex_t re;
  int answer = 1;
  if ((*flags)->i) {
    regcomp(&re, *pattern, REG_ICASE);
  } else {
    regcomp(&re, *pattern, 0);
  }
  answer = regexec(&re, str, 0, NULL, 0);
  regfree(&re);
  return answer;
}

void free_pat_o(Node_pat_o **head_pat_o) {
  if (*head_pat_o) {
    do {
      Node_pat_o *tmp = *head_pat_o;
      regfree(&(tmp->reg));
      (*head_pat_o) = (*head_pat_o)->next_pat_o;
      free(tmp);
    } while (*head_pat_o);
  }
}

void free_pat_file(Node_file **head_file, Node_pat **head_pat) {
  if (*head_file) {
    do {
      Node_file *tmp = *head_file;
      (*head_file) = (*head_file)->next_file;
      free(tmp);
    } while (*head_file);
  }
  if (*head_pat) {
    do {
      Node_pat *tmp = *head_pat;
      if (tmp->pat_file) {
        free(tmp->pat);
      }
      (*head_pat) = (*head_pat)->next_pat;
      free(tmp);
    } while (*head_pat);
  }
}

void move(Node_file **head_file, Node_pat **head_pat, int *pat_count) {
  Node_file *tmp_file = *head_file;
  Node_pat *tmp_pat = (Node_pat *)malloc(sizeof(Node_pat));
  tmp_pat->next_pat = NULL;
  tmp_pat->pat = tmp_file->file;
  *head_pat = tmp_pat;
  *head_file = tmp_file->next_file;
  *pat_count = *pat_count + 1;
  free(tmp_file);
}

void push_file(char *argv, Node_file ***head_file) {
  Node_file *tmp = (Node_file *)malloc(sizeof(Node_file));
  if (*(*head_file) != NULL) {
    Node_file *last = *(*head_file);
    while (last->next_file != NULL) {
      last = last->next_file;
    }
    tmp->file = argv;
    tmp->next_file = NULL;
    last->next_file = tmp;
  } else {
    tmp->file = argv;
    tmp->next_file = NULL;
    *(*head_file) = tmp;
  }
}

void push_pattern(char *argv, Node_pat ***head_pat, int **pat_count) {
  Node_pat *tmp = (Node_pat *)malloc(sizeof(Node_pat));
  if (*(*head_pat) != NULL) {
    Node_pat *last = *(*head_pat);
    while (last->next_pat != NULL) {
      last = last->next_pat;
    }
    tmp->pat_file = 0;
    tmp->pat = argv;
    tmp->next_pat = NULL;
    last->next_pat = tmp;
  } else {
    tmp->pat_file = 0;
    tmp->pat = argv;
    tmp->next_pat = NULL;
    *(*head_pat) = tmp;
  }
  *(*pat_count) = *(*pat_count) + 1;
}

int push_pattern_from_file(char *argv, Node_pat ***head_pat, int **pat_count) {
  FILE *f = fopen(argv, "r");
  char buf[4096];
  int len = 0;
  int res = 1;
  if (f != NULL) {
    while (!(feof(f))) {
      fgets(buf, 4095, f);
      for (char *i = buf; *i != '\0'; i++) {
        if (*i == '\n') {
          *i = *(i + 1);
          break;
        }
      }
      len = strlen(buf);
      Node_pat *tmp = (Node_pat *)malloc(sizeof(Node_pat));
      if (*(*head_pat) != NULL) {
        Node_pat *last = *(*head_pat);
        while (last->next_pat != NULL) {
          last = last->next_pat;
        }
        tmp->pat = malloc((len + 1) * sizeof(char));
        for (int i = 0; i <= len; i++) {
          tmp->pat[i] = buf[i];
        }
        tmp->pat_file = 1;
        tmp->next_pat = NULL;
        last->next_pat = tmp;
      } else {
        tmp->pat = malloc((len + 1) * sizeof(char));
        for (int i = 0; i != len; i++) {
          tmp->pat[i] = buf[i];
        }
        tmp->pat_file = 1;
        tmp->next_pat = NULL;
        *(*head_pat) = tmp;
      }
    }
    fclose(f);
    *(*pat_count) = *(*pat_count) + 1;
  } else {
    res = 0;
  }
  return res;
}
