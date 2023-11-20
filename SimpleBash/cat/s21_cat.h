#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#ifndef CAT_S21_CAT_H
#define CAT_S21_CAT_H

typedef struct {
  int b_flag;  // numbering non empty strings
  int e_flag;  // shows /n as a $
  int n_flag;  // numbering all strings
  int s_flag;  // sqeezing some neibors empty strings
  int t_flag;  // shows TAB as a ^|
  int v_flag;
} flag_info;

void cat_no_args();
void cat_no_flag(char *argv);
void cat_args(int argc, char *argv[]);
int cat_parse_args(char *argv, flag_info *flags, char *err_flag);
void cat_show(flag_info *flags, char *filename);
void use_flag(char *c, const char cprev, flag_info *flags, int *count,
              int *s_be, int q);
void no_print_symbols(int c);

#endif  // CAT_S21_CAT_H
