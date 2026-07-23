#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "tush.h"

#define TUSH_VERSION_NO "1.0.0"

// GLOBAL VARIABLES
char *g_prompt;        // prompt is global so users can change it

// List of builtin commands, followed by their corresponding functions.
char *builtin_str[] = {
    "cd",
    "help",
    "prompt",
    "exit"
};

int (*builtin_func[]) (char **) = {
    &tush_cd,
    &tush_help,
    &tush_prompt,
    &tush_exit
};

char *tush_read_line(void)
{
  char *line = NULL;
  size_t bufsize = 0;

  if (-1 == getline(&line, &bufsize, stdin)) {
    if (feof(stdin)) {
      exit(EXIT_SUCCESS); // EOF ctrl-D
    } else {
      perror("readline");
      exit(EXIT_FAILURE);
    }
  }

  return line;
}

#define TUSH_TOK_BUFSIZE 64
#define TUSH_TOK_DELIM " \t\r\n\a"
char **tush_split_line(char *line)
{
    int position = 0;
    int bufsize = TUSH_TOK_BUFSIZE;
    char *token, **tokens_backup;
    char **tokens = malloc(bufsize * sizeof(char*));

    if (!tokens) {
        fprintf(stderr, "tush: failed to allocate token buffer\n");
        exit(EXIT_FAILURE);
    }

    // Get the first token
    token = strtok(line, TUSH_TOK_DELIM);

    // Get the remaining tokens, if any
    while (token != NULL) {
        // Fill our tokens array
        tokens[position] = token;
        position++;

        // Reallocate buffer if we run out of space
        if (position >= bufsize) {
            bufsize += TUSH_TOK_BUFSIZE;
            tokens_backup = tokens;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) {
                free(tokens_backup);
                fprintf(stderr, "tush: failed to reallocate token buffer\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, TUSH_TOK_DELIM);
    }

    tokens[position] = NULL;
    return tokens;
}

int tush_launch(char **args)
{
    int status;
    pid_t pid;

    pid = fork();

    if (-1 == pid) {
	// Fork failed
	fprintf(stderr, "tush: fork failed\n");
	exit(EXIT_FAILURE);

    } else if (0 == pid) {  
	// Child process executes command
	execvp(args[0], args);
	fprintf(stderr, "%s: command not found\n", args[0]);
	exit(EXIT_FAILURE);

    } else {
	// Parent waits for child to finish
	do {
	    waitpid(pid, &status, WUNTRACED);
	} while (!WIFEXITED(status) && !WIFSIGNALED(status));

    }

    return 1;
}

int tush_execute(char **args)
{
    if (args[0] == NULL) {
	// An empty command was entered.
	return 1;
    }

    for (int i = 0; i < tush_num_builtins(); i++) {
	if (strcmp(args[0], builtin_str[i]) == 0) {
	    return (*builtin_func[i])(args);
	}
    }

    return tush_launch(args);
}

int tush_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

// Builtin function implementations
int tush_cd(char **args)
{
    if (args[1] == NULL) {
	fprintf(stderr, "cd: expected an argument\n");
	return -1;
    }

    chdir(args[1]);

    return 1;
}

int tush_help(char **args)
{
    // TODO finish me
    (void)args;
    printf("Jason Turley's Turtle Shell (tush), version %s\n\n", TUSH_VERSION_NO);
    printf("Usage: ./tush\n");
    printf("Simply type program names and press enter to run them.\n");
    printf("This shell includes the following builtin functions:\n");

    for (int i = 0; i < tush_num_builtins(); i++) {
	printf("\t%s\n", builtin_str[i]);
    }

    printf("For help with other programs, use the man command.\n");

    return 1;
}


int tush_prompt(char **args)
{
    if (args[1] == NULL) {
	fprintf(stderr, "prompt: enter new prompt\n");
	return 1;
    }

    g_prompt = strdup(args[1]);
    if (g_prompt == NULL) {
	fprintf(stderr, "prompt: failed to allocate memory for new prompt\n");
	return -1;
    }
    return 1;
}

int tush_exit(char **args)
{
    (void)args;
    return 0;
}

void tush_loop(void)
{
    char *line;
    char **args;
    char *cwd;
    int status;

    g_prompt = (char *)malloc(64 * sizeof(char));

    do {
	cwd = getcwd(NULL, 0);
	sprintf(g_prompt, "(pid=%d)%s$ ", getpid(), cwd);
        printf("%s", g_prompt);

        line = tush_read_line();
        args = tush_split_line(line);
        status = tush_execute(args);


        free(line);
        line = NULL;
        free(args);
        args = NULL;
	free(cwd);
	cwd = NULL;

    } while (status);

    free(g_prompt);
}

int main()
{

    tush_loop();

    return EXIT_SUCCESS;
}
