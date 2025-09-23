#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

        // ---------- Step 2: Built-in cd ----------
        if (strcmp(args[0], "cd") == 0) {
            if (argc == 1) {
                // No argument -> go to HOME
                char *home = getenv("HOME");
                if (home == NULL) home = "/";
                if (chdir(home) != 0) {
                    perror("cd failed");
                }
            } else {
                // Change to specified directory
                if (chdir(args[1]) != 0) {
                    perror("cd failed");
                }
            }
            continue; // skip fork/exec
        }

        // ---------- Step 1: External commands ----------
        pid_t pid = fork();
        if (pid == 0) {
            execvp(args[0], args);
            perror("Execution failed");
            exit(1);
        } else if (pid > 0) {
            waitpid(pid, NULL, 0);
        } else {
            perror("Fork failed");
        }
    }

    return 0;
}
