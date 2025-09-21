#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

int parse_input(char *input, char **args) {
    int i = 0;
    args[i] = strtok(input, " ");
    while (args[i] != NULL && i < MAX_ARGS - 1) {
        i++;
        args[i] = strtok(NULL, " ");
    }
    return i;
}

int main() {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];
    char *args_pipe[MAX_ARGS];

    while (1) {
        printf("smart-shell> ");
        fflush(stdout);

        if (!fgets(input, MAX_INPUT, stdin)) break;
        input[strcspn(input, "\n")] = 0;

        if (input[0] == '\0') continue;
        if (strcmp(input, "exit") == 0) break;

        // Check if there is a pipe
        char *pipe_pos = strchr(input, '|');
        if (pipe_pos) {
            // Split input into two commands
            *pipe_pos = '\0';
            char *cmd1 = input;
            char *cmd2 = pipe_pos + 1;

            int argc1 = parse_input(cmd1, args);
            int argc2 = parse_input(cmd2, args_pipe);

            int pipefd[2];
            if (pipe(pipefd) == -1) { perror("pipe"); exit(1); }

            pid_t pid1 = fork();
            if (pid1 == 0) {
                // First child - output to pipe
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);

                // Handle I/O redirection for first command
                for (int i = 0; args[i] != NULL; i++) {
                    if (strcmp(args[i], ">") == 0) {
                        args[i] = NULL;
                        int fd = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                        if (fd < 0) { perror("Failed to open file"); exit(1); }
                        dup2(fd, STDOUT_FILENO);
                        close(fd);
                        break;
                    }
                    if (strcmp(args[i], "<") == 0) {
                        args[i] = NULL;
                        int fd = open(args[i+1], O_RDONLY);
                        if (fd < 0) { perror("Failed to open file"); exit(1); }
                        dup2(fd, STDIN_FILENO);
                        close(fd);
                        break;
                    }
                }

                execvp(args[0], args);
                perror("Execution failed");
                exit(1);
            }

            pid_t pid2 = fork();
            if (pid2 == 0) {
                // Second child - input from pipe
                dup2(pipefd[0], STDIN_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);

                // Handle I/O redirection for second command
                for (int i = 0; args_pipe[i] != NULL; i++) {
                    if (strcmp(args_pipe[i], ">") == 0) {
                        args_pipe[i] = NULL;
                        int fd = open(args_pipe[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                        if (fd < 0) { perror("Failed to open file"); exit(1); }
                        dup2(fd, STDOUT_FILENO);
                        close(fd);
                        break;
                    }
                    if (strcmp(args_pipe[i], "<") == 0) {
                        args_pipe[i] = NULL;
                        int fd = open(args_pipe[i+1], O_RDONLY);
                        if (fd < 0) { perror("Failed to open file"); exit(1); }
                        dup2(fd, STDIN_FILENO);
                        close(fd);
                        break;
                    }
                }

                execvp(args_pipe[0], args_pipe);
                perror("Execution failed");
                exit(1);
            }

            // Parent
            close(pipefd[0]);
            close(pipefd[1]);
            waitpid(pid1, NULL, 0);
            waitpid(pid2, NULL, 0);
        }
        else {
            // No pipe, normal execution (like Step 3)
            int argc = parse_input(input, args);
            if (argc == 0) continue;

            pid_t pid = fork();
            if (pid == 0) {
                // Handle I/O redirection
                for (int i = 0; args[i] != NULL; i++) {
                    if (strcmp(args[i], ">") == 0) {
                        args[i] = NULL;
                        int fd = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                        if (fd < 0) { perror("Failed to open file"); exit(1); }
                        dup2(fd, STDOUT_FILENO);
                        close(fd);
                        break;
                    }
                    if (strcmp(args[i], "<") == 0) {
                        args[i] = NULL;
                        int fd = open(args[i+1], O_RDONLY);
                        if (fd < 0) { perror("Failed to open file"); exit(1); }
                        dup2(fd, STDIN_FILENO);
                        close(fd);
                        break;
                    }
                }

                execvp(args[0], args);
                perror("Execution failed");
                exit(1);
            }
            else if (pid > 0) {
                waitpid(pid, NULL, 0);
            }
            else {
                perror("Fork failed");
            }
        }
    }

    return 0;
}
