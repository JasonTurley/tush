#ifndef SHELL_H_
#define SHELL_H_


/*
 *  @brief Reads a line of input from standard input.
 *  @return Returns the line read.
 */
char *tush_read_line(void);

/*
 *  @brief Splits a line into tokens using whitespace as a delimiter.
 *  @param line The line to parse into tokens.
 *  @return Null-terminated array of tokens.
 */
char **tush_split_line(char *line);

/*
 *  @brief Executes command line arguments.
 *  @param args Null-terminated list of arguments.
 */
int tush_execute(char **args);

/*
 *  @brief The main loop for getting input, parsing it, and executing it.
 */
void tush_loop(void);

/*
 *  @brief Launches a child process to execute a program.
 *  @param args Null-terminated list of arguments.
 */
int tush_launch(char **args);

/*
 *  Function declarations for builtin shell commands
 */
int tush_cd(char **args);
int tush_help(char **args);
int tush_prompt(char **args);
int tush_exit(char **args);

int tush_num_builtins(void);

#endif // SHELL_H_
