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

/**
 * @brief List's node that content option's info
 * @param pat pointer to string that contents options
 * @param pat_file variable that indicates option from input or file
 * @param next_pat pointer to the next list's node
 */
typedef struct Node_pat {
  char *pat;
  int pat_file;
  struct Node_pat *next_pat;
} Node_pat;

/**
 * @brief List's node that content file's info
 * @param file pointer to string that contents filename
 * @param next_file pointer to the next list's node
 */
typedef struct Node_file {
  char *file;
  struct Node_file *next_file;
} Node_file;

/**
 * @brief List's node that content regular expressions info
 * @param reg regular expression
 * @param next_file pointer to the next list's node
 */
typedef struct Node_pat_o {
  regex_t reg;
  struct Node_pat_o *next_pat_o;
} Node_pat_o;

/**
 * @brief Main parcing function from input
 * @param argc quantity of arguments
 * @param argv pointer to arguments
 * @param head_pat pointer to option's list
 * @param head_file pointer to file's list
 * @param flags pointer to struct with option's info
 * @param pat_count options counter
 * @param files_counter files counter
 * @return error code 1 -ok , 0 - error
 */
int parse_args(int *argc, char *argv[], Node_pat **head_pat,
               Node_file **head_file, flags_info *flags, int *pat_count,
               int *files_counter);

/**
 * @brief Function to add node at file's list
 * @param argv pointer to agrument
 * @param head_file pointer to file's list
 */
void push_file(char *argv, Node_file ***head_file);

/**
 * @brief Function to add node at option's list
 * @param argv pointer to agrument
 * @param head_pat pointer to option's list
 * @param pat_count options counter
 */
void push_pattern(char *argv, Node_pat ***head_pat, int **pat_count);

/**
 * @brief Function to add node at option's list reading options from file
 * @param argv pointer to agrument
 * @param head_pat pointer to option's list
 * @param pat_count options counter
 * @return error code 1 - ok, 0 - file not found
 */
int push_pattern_from_file(char *argv, Node_pat ***head_pat, int **pat_count);

/**
 * @brief Supportin function for parcing
 * @param argv pointer to agrument
 * @param flags pointer to struct with option's info
 * @param err_flag error flag
 * @param is_pat_e indicator that optione 'e' was before
 * @param is_pat_f indicator that optione 'f' was before
 * @param ef_opt indicator that optione 'e' and 'f' wasn't before
 * @param opt pointer to arument
 * @param ef_was indicator that optione 'e' and 'f' was before
 * @return error code 1 - ok, 0 - error
 */
int parcing(char *argv, flags_info **flags, char *err_flag, int *is_pat_e,
            int *is_pat_f, int *ef_opt, char **opt, int *ef_was);

/**
 * @brief Function that move node from file's list to option's list
 * @param head_file pointer to file's list
 * @param head_pat pointer to options's list
 * @param pat_count pointer to options counter
 */
void move(Node_file **head_file, Node_pat **head_pat, int *pat_count);

/**
 * @brief Main logical function
 * @param flags pointer to struct with option's info
 * @param head_file pointer to file's list
 * @param head_pat pointer to options's list
 * @param files_counter files counter
 * @param flag_counter quantity of parced different options
 * @param pat_count pointer to options counter
 */
void grep_func(flags_info *flags, Node_file **head_file, Node_pat **head_pat,
               int *files_counter, int *flag_counter, int *pat_count);

/**
 * @brief Support logic function
 * @param cmp_res result of compare by regex
 * @param flags pointer to struct with option's info
 * @param file pointer to grepping filename
 * @param h_flag indicator that 'h' option was or quantity of grepping files is
 * 1
 * @param line_counter line's counter
 * @param str pointer to grepping string of file
 * @param flag_counter pointer to quantity of parced different options
 */
void grep_logic(int *cmp_res, flags_info **flags, char **file, int *h_flag,
                int *line_counter, char *str, int **flag_counter);

/**
 * @brief Function that implement logic 'l' option
 * @param flags pointer to struct with option's info
 * @param h_flag indicator that 'h' option was or quantity of grepping files is
 * 1
 * @param bingo_counter pointer to support variable for logic 'l', 'c', 'o'
 * options
 * @param bingo_str pointer to support variable for logic 'l', 'c', 'o' option
 */
void logic_l(flags_info **flags, int *h_flag, int *bingo_counter, char **file,
             int *bingo_str);

/**
 * @brief Function that implement logic 'c' option
 * @param flags pointer to struct with option's info
 * @param h_flag indicator that 'h' option was or quantity of grepping files is
 * 1
 * @param bingo_counter pointer to support variable for logic 'l', 'c', 'o'
 * options
 * @param file pointer to grepping filename
 * @param line_counter line's counter
 * @param bingo_str pointer to support variable for logic 'l', 'c', 'o' option
 */
void logic_c(flags_info **flags, int *h_flag, int *bingo_counter, char **file,
             int *line_counter, int *bingo_str);

/**
 * @brief Function that check string by regex
 * @param str pointer to checking string
 * @param pattern pointer to options
 * @param flags pointer to struct with option's info
 * @return count of succesful comparing
 */
int regex_func(char *str, char **pattern, flags_info **flags);

/**
 * @brief Function implementing logic of 'o' option
 * @param str pointer to string
 * @param head_pat_o pointer to list of regular expression
 * @param flags pointer to struct with option's info
 * @param file pointer to grepping filename
 * @param line_counter line's counter
 * @param h_flag indicator that 'h' option was or quantity of grepping files is
 * 1
 * @return count of succesful comparing
 */
int regex_func_o(char *str, Node_pat_o **head_pat_o, flags_info **flags,
                 char **file, int *line_counter, int *h_flag);

/**
 * @brief Function that init list of regular expression
 * @param head_pat_o pointer to list of regular expression
 * @param head_pat pointer to option's list
 * @param flags pointer to struct with option's info
 */
void init_reg_o(Node_pat_o **head_pat_o, Node_pat ***head_pat,
                flags_info **flags);

/**
 * @brief Fuction that free resources
 * @param head_file pointer to file's list
 * @param head_pat pointer to options's list
 */
void free_pat_file(Node_file **head_file, Node_pat **head_pat);

/**
 * @brief Function that free resources of reg's list
 * @param head_pat_o pointer to list of regular expression
 */
void free_pat_o(Node_pat_o **head_pat_o);

#endif  // GREP_S21_GREP_H