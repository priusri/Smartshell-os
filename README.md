# Smartshell OS

A progressive implementation of a Unix-like shell in C, demonstrating core shell functionality through incremental steps. Each step builds upon the previous one, adding new features and capabilities.

## Overview

Smartshell OS is an educational project that implements a command-line shell with features commonly found in Unix/Linux shells like `bash` or `sh`. The project is organized into progressive steps, each adding new functionality to the shell.

## Project Structure

### Core Implementation Steps

| File | Features |
|------|----------|
| **smartshell_step1.c** | Basic command parsing and tokenization |
| **smartshell_step1_bg.c** | Background process execution support (`&`) |
| **smartshell_step2.c** | Command execution with `fork()` and `execvp()` |
| **smartshell_step2_cd.c** | Built-in `cd` command for directory navigation |
| **smartshell_step3.c** | I/O redirection support (`>` for output) |
| **smartshell_step4.c** | Pipe support (`|`) for command chaining |

## Features by Step

### Step 1: Input Parsing
- Accepts user input from the command line
- Parses input into tokens (command and arguments)
- Displays parsed tokens for verification
- Command: `exit` to terminate the shell

```
smart-shell> ls -la
Tokens:
arg[0] = ls
arg[1] = -la
```

### Step 1 Variant: Background Processes
- Inherits all Step 1 features
- Supports background execution with `&` operator
- Allows multiple processes to run concurrently

```
smart-shell> sleep 10 &
```

### Step 2: Command Execution
- Executes system commands using `fork()` and `execvp()`
- Creates child processes to run commands
- Parent process waits for command completion
- Full Unix command support

```
smart-shell> pwd
smart-shell> date
smart-shell> ls
```

### Step 2 Variant: Built-in Commands
- Implements the `cd` (change directory) command as a built-in
- Handles directory navigation: `cd /path/to/directory`
- Returns to home directory with `cd` (no arguments)
- Built-in commands are executed in the current shell process

```
smart-shell> cd /home/user
smart-shell> pwd
/home/user
```

### Step 3: I/O Redirection
- Supports output redirection with `>` operator
- Redirects standard output to files
- Creates files if they don't exist; truncates if they do
- Inherits all previous features

```
smart-shell> echo "Hello World" > output.txt
smart-shell> ls > file_list.txt
```

### Step 4: Pipe Support
- Implements command piping with `|` operator
- Chains multiple commands together
- Output of first command becomes input to second
- Supports full pipeline functionality

```
smart-shell> ls | grep .c
smart-shell> ps aux | grep bash
smart-shell> cat file.txt | wc -l
```

## Compilation

Each file is a standalone implementation and can be compiled independently:

```bash
# Step 1
gcc -o smartshell_step1 smartshell_step1.c

# Step 1 with Background Processes
gcc -o smartshell_step1_bg smartshell_step1_bg.c

# Step 2
gcc -o smartshell_step2 smartshell_step2.c

# Step 2 with cd command
gcc -o smartshell_step2_cd smartshell_step2_cd.c

# Step 3
gcc -o smartshell_step3 smartshell_step3.c

# Step 4
gcc -o smartshell_step4 smartshell_step4.c
```

## Usage

Run any compiled version:

```bash
./smartshell_step1
./smartshell_step4    # Most feature-complete version
```

The shell will display the prompt:
```
smart-shell>
```

Enter commands as you would in any Unix shell:

```
smart-shell> command [args]
```

Exit the shell by typing:
```
smart-shell> exit
```

## Technical Highlights

### Key Concepts Demonstrated

- **Process Management**: Using `fork()` to create child processes
- **Process Execution**: Using `execvp()` to execute programs
- **Process Synchronization**: Using `waitpid()` to wait for process completion
- **File I/O**: Using `open()`, `dup2()`, and file descriptors for redirection
- **Pipes**: Using `pipe()` system call for inter-process communication
- **String Parsing**: Tokenizing input with `strtok()` for command parsing
- **Signal Handling**: Managing background processes with the `&` operator

### Constants

```c
#define MAX_INPUT 1024    // Maximum input line length
#define MAX_ARGS 64       // Maximum number of arguments per command
```

## Limitations & Future Enhancements

### Current Limitations
- No error handling for malformed commands
- Limited built-in command support (only `cd` in step 2_cd variant)
- No command history or line editing
- No environment variable expansion
- No glob pattern expansion (wildcards)
- No signal handling (e.g., `Ctrl+C`)

### Potential Enhancements
- Add more built-in commands (`echo`, `pwd`, `exit` variants)
- Implement command history
- Add input/output redirection (`<`, `>>`, `2>`)
- Support for aliases
- Environment variable support
- Job control and process management
- Signal handlers for `SIGINT`, `SIGTERM`

## Educational Value

This project is ideal for learning:
- Unix/Linux shell internals
- Process creation and management
- Inter-process communication
- System programming in C
- Operating system concepts

## Compilation Flags (Optional)

For additional warnings and debugging:

```bash
gcc -Wall -Wextra -g -o smartshell_step4 smartshell_step4.c
```

## System Requirements

- Unix/Linux environment (Linux, macOS, or WSL on Windows)
- GCC compiler or compatible C compiler
- POSIX-compliant system

## License

This project is provided as-is for educational purposes.

## Author Notes

This implementation follows a progressive learning approach where each step introduces new shell features, building from basic command parsing to advanced features like pipes and redirection. Students can study each version to understand how shells work at the system level.
