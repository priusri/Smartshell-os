#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h> // For file operations

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

    while (1) {
        printf("smart-shell> ");
        fflush(stdout);

        if (!fgets(input, MAX_INPUT, stdin)) break;
        input[strcspn(input, "\n")] = 0;

        if (input[0] == '\0') continue;
        if (strcmp(input, "exit") == 0) break;

        int argc = parse_input(input, args);
        if (argc == 0) continue;

        pid_t pid = fork();
        if (pid == 0) {
            // Child process – handle I/O redirection

            for (int i = 0; args[i] != NULL; i++) {
                if (strcmp(args[i], ">") == 0) {
                    args[i] = NULL; // Remove '>'
                    int fd = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if (fd < 0) { perror("Failed to open file"); exit(1); }
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                    break;
                }
                if (strcmp(args[i], "<") == 0) {
                    args[i] = NULL; // Remove '<'
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

    return 0;
}
