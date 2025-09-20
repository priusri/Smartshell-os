#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

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

        pid_t pid = fork();  // Create child process

        if (pid == 0) {
            // Child process executes the command
            execvp(args[0], args);
            perror("Execution failed");  // Only if exec fails
            exit(1);
        } 
        else if (pid > 0) {
            // Parent waits for child
            waitpid(pid, NULL, 0);
        } 
        else {
            perror("Fork failed");
        }
    }

    return 0;
}
