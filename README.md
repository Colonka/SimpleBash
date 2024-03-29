# SimpleBash

Development of Bash text utilities: cat, grep.

P.s. This is my one of the the first project so there is many things is not ideal...
And I've decided not improve this code for more indicative track of my code improvement!

## Description
In this project I've developed basic Bash utilities for working with C programming language texts. 
These utilities (cat and grep) are often used in the Linux terminal. 
In addition I've made bash scripts for testing.

## Insructions for documentation

To get documentation make command `make dvi` in terminal being in `cat` or `grep` folder

### cat Usage

Cat is one of the most frequently used commands on Unix-like operating systems. It has three related functions with regard to text files: displaying them, combining copies of them and creating new ones.

`cat [OPTION] [FILE]...`

### cat Options

| No. | Options | Description |
| ------ | ------ | ------ |
| 1 | -b (GNU: --number-nonblank) | numbers only non-empty lines |
| 2 | -e implies -v (GNU only: -E the same, but without implying -v) | but also display end-of-line characters as $  |
| 3 | -n (GNU: --number) | number all output lines |
| 4 | -s (GNU: --squeeze-blank) | squeeze multiple adjacent blank lines |
| 5 | -t implies -v (GNU: -T the same, but without implying -v) | but also display tabs as ^I  |

### grep Usage

`grep [options] template [file_name]`

### grep Options

| No. | Options | Description |
| ------ | ------ | ------ |
| 1 | -e | pattern |
| 2 | -i | Ignore uppercase vs. lowercase.  |
| 3 | -v | Invert match. |
| 4 | -c | Output count of matching lines only. |
| 5 | -l | Output matching files only.  |
| 6 | -n | Precede each matching line with a line number. |
| 7 | -h | Output matching lines without preceding them by file names. |
| 8 | -s | Suppress error messages about nonexistent or unreadable files. |
| 9 | -f file | Take regexes from a file. |
| 10 | -o | Output the matched parts of a matching line. |

## Insructions for running

More information about using cat, grep you can find here: 
- cat: https://man7.org/linux/man-pages/man1/cat.1.html
- grep: https://man7.org/linux/man-pages/man1/grep.1.html

### Cat:

- Being in "cat" folder make command `make` in terminal.
- Then execute s21_cat with necessary options.
- You can list options and then files by space

template:    `./s21_cat -<OPTION> ... -<OPTION> <FILENAME> ... <FILENAME>`

example: `./s21_cat -n -s file_1.txt file_2.txt`

### Grep:

- Being in "grep" folder make command `make` in terminal.
- Then execute s21_grep with necessary options.
- You can list options by space and combine without space and then files by space.

template: `./s21_grep -<OPTION><OPTION> ... -<OPTION> <FILENAME> ... <FILENAME>`

example: `./s21_grep -iv -h file_1.txt file_2.txt`
