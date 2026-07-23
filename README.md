# tush — Turtle Shell

A simplistic shell implementation built from scratch in C to learn systems programming concepts.

## Overview

**tush** (Turtle Shell) is an educational shell implementation designed to explore fundamental systems programming concepts like process management (`fork`, `exec`, `wait`), command parsing, and shell loop architecture. While minimal in features compared to production shells like bash or zsh, tush demonstrates the core mechanics of how shells work.

## Purpose

This project serves as a hands-on learning tool for understanding:

- **Process creation and management** — Using `fork()`, `execvp()`, and `waitpid()` to spawn and manage child processes
- **Command parsing** — Tokenizing user input into command and arguments
- **Shell loop architecture** — The read-parse-execute loop that defines shell behavior
- **Built-in commands** — Implementing shell-internal commands distinct from external programs
- **Dynamic prompt customization** — Managing global state within the shell process

## Building

### Requirements

- GCC or Clang
- POSIX-compliant system (Linux, macOS, etc.)

### Compilation

The repo contains a Makefile with my preferred warning flags for easy compilation.

```bash
make
```

Alternatively, you can manually run gcc or clang.


```bash
gcc -o tush shell.c
```

Or with warnings enabled:

```bash
gcc -Wall -Wextra -o tush shell.c
```

## Usage

Run the shell:

```bash
./tush
```

You'll see the default prompt `▶  ` and can enter commands just like in bash or zsh:

```
▶  ls -la
▶  echo "Hello, World!"
▶  whoami
```

### Built-in Commands

- **`cd <directory>`** — Change the current working directory
- **`help`** — Display information about tush and available built-in commands
- **`prompt <new_prompt>`** — Change the shell prompt to a custom string
- **`exit`** — Terminate the shell

### External Commands

Any command not in the built-in list is executed as an external program. The shell uses `execvp()` to search for the program in your `$PATH`.

```
▶  echo "This runs /bin/echo"
This runs /bin/echo
▶  cd /tmp
▶  pwd
/tmp
▶  exit
```

## Project Structure

```
tush.c          Main shell implementation
tush.h          Function declarations and constants
README.md       This file
Makefile        Recipie to compile the binary
```

## Implementation Details

### Core Components

**`tush_loop()`** — The main shell loop. Repeatedly:
1. Displays the prompt
2. Reads a line of input
3. Parses the line into tokens
4. Executes the command

**`tush_read_line()`** — Reads user input from stdin using `getline()`, handling EOF gracefully.

**`tush_split_line()`** — Tokenizes input on whitespace, building a null-terminated argument array.

**`tush_execute()`** — Routes commands to built-in handlers or `tush_launch()` for external programs.

**`tush_launch()`** — Forks a child process and uses `execvp()` to load and run an external program. The parent process waits for the child to complete using `waitpid()`.

### Key Design Decisions

- **Global prompt** — `g_prompt` is a global variable allowing built-in commands to modify it
- **String duplication** — `tush_prompt()` uses `strdup()` to copy the new prompt, preventing use-after-free when the token buffer is freed
- **Consistent naming** — All functions use the `tush_` prefix for clear namespace ownership and to avoid naming collisions

## Learning References

This implementation is inspired by [Stephan Brennan's shell tutorial](https://brennan.io/2015/01/16/write-a-shell-in-c/), an excellent resource for understanding shell internals and process management in Unix-like systems.

## Future Enhancements

Potential extensions for deeper learning:

- **Signal handling** — Handle Ctrl+C, Ctrl+Z with SIGINT and SIGTSTP
- **Pipes and redirection** — Implement `|`, `>`, `<` operators
- **Background processes** — Support `&` to run commands asynchronously
- **Command history** — Store and navigate previous commands
- **Job control** — Manage multiple background jobs
- **Pretty colors** — Add colored output

## Author

Jason Turley

## License

Educational use. Feel free to modify and learn from this code.
