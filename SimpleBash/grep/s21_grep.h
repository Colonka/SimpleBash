#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef GREP_S21_GREP_H
#define GREP_S21_GREP_H

typedef struct {
  int e;  // pattern
  int i;  // ignor register
  int v;  // show strings without argv
  int c;  // print number of finding strings
  int l;  // print only files with insertion
  int n;  // numering finding strings
  int h;  // don't print file's name
  int s;  // don't print error messages
  int f;  // pattern from file
  int o;  // print only insertions
} flags_info;

typedef struct Node_pat {
  char *pat;
  int pat_file;
  struct Node_pat *next_pat;
} Node_pat;

typedef struct Node_file {
  char *file;
  struct Node_file *next_file;
} Node_file;

typedef struct Node_pat_o {
  regex_t reg;
  struct Node_pat_o *next_pat_o;
} Node_pat_o;

void push_file(char *argv, Node_file ***head_file);
void push_pattern(char *argv, Node_pat ***head_pat, int **pat_count);
int push_pattern_from_file(char *argv, Node_pat ***head_pat, int **pat_count);
int parse_args(int *argc, char *argv[], Node_pat **head_pat,
               Node_file **head_file, flags_info *flags, int *pat_count,
               int *files_counter);
int parcing(char *argv, flags_info **flags, char *err_flag, int *is_pat_e,
            int *is_pat_f, int *ef_opt, char **opt, int *ef_was);
void move(Node_file **head_file, Node_pat **head_pat, int *pat_count);
void grep_func(flags_info *flags, Node_file **head_file, Node_pat **head_pat,
               int *files_counter, int *flag_counter, int *pat_count);
void grep_logic(int *cmp_res, flags_info **flags, char **file, int *h_flag,
                int *line_counter, char *str, int **flag_counter);
int regex_func(char *str, char **pattern, flags_info **flags);
int regex_func_o(char *str, Node_pat_o **head_pat_o, flags_info **flags,
                 char **file, int *line_counter, int *h_flag);
void init_reg_o(Node_pat_o **head_pat_o, Node_pat ***head_pat,
                flags_info **flags);
void free_pat_file(Node_file **head_file, Node_pat **head_pat);
void free_pat_o(Node_pat_o **head_pat_o);
void logic_l(flags_info **flags, int *h_flag, int *bingo_counter, char **file,
             int *bingo_str);
void logic_c(flags_info **flags, int *h_flag, int *bingo_counter, char **file,
             int *line_counter, int *bingo_str);

#endif  // GREP_S21_GREP_H