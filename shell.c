#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // fork, execvp
#include <sys/wait.h>   // waitpid
#include <string.h>

#define MAX 1024

int main() {
    char input[MAX];

    while (1) {
        printf("mysh> ");
        fflush(stdout);

        if (!fgets(input, MAX, stdin)) break;

        input[strcspn(input, "\n")] = 0;

        // exit
        if (strcmp(input, "exit") == 0) break;

        // Tokenize input into args
        char *args[64];
        int i = 0;
        char *token = strtok(input, " ");
        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        // Fork 
        pid_t pid = fork();

        if (pid == 0) {
            // Child 
            execvp(args[0], args);
            perror("exec failed");
            exit(1);
        } 
        else if (pid > 0) {
            // Parent 
            waitpid(pid, NULL, 0);
        } 
        else {
            perror("fork failed");
        }
    }

    return 0;
}
