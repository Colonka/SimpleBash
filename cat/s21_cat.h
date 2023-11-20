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
  int v_flag;  //
} flag_info;

/**
 * @brief Function that simply print content of files in console without
 * transformation
 * @param argv pointer to string file name
 */
void cat_no_flag(char *argv);

/**
 * @brief Function that make main transmormations
 * @param argc quantity of argument
 * @param argv pointer to array of strings that content arguments
 */
void cat_args(int argc, char *argv[]);

/**
 * @brief Function that parse argments
 * @param argv pointer to string that content option
 * @param flags ponter to struct that content info about options
 * @param err_flag pointer to error indicator
 * @return error's code: 1 - ok, 0 - error
 */
int cat_parse_args(char *argv, flag_info *flags, char *err_flag);

/**
 * @brief Function that print content of files in console with tansformations
 * @param flags ponter to struct that content info about options
 * @param filename pointer to string that content file's name
 */
void cat_print(flag_info *flags, char *filename);

/**
 * @brief Function that implements logic of printing content of files in console
 * @param c pointer to symbol in string
 * @param cprev pointer to symbol before "c" in string
 * @param flags ponter to struct that content info about options
 * @param count pointer to quantity of printing symbols
 * @param s_be pointer to indicator of "s" option
 * @param q first call function indicator
 */
void use_flag(char *c, const char cprev, flag_info *flags, int *count,
              int *s_be, int q);

/**
 * @brief Function that print noprint symbols in console
 * @param c pointer to symbol in string
 */
void no_print_symbols(int c);

#endif  // CAT_S21_CAT_H
