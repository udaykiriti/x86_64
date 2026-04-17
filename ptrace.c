#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>   // user_regs_struct
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execl("/bin/ls", "ls", NULL);
    } 
    else {
        int status;
        waitpid(pid, &status, 0);

        while (1) {
            // Let child run until next syscall
            ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
            waitpid(pid, &status, 0);

            if (WIFEXITED(status)) break;

            struct user_regs_struct regs;
            ptrace(PTRACE_GETREGS, pid, NULL, &regs);

            printf("Syscall number: %lld\n", regs.orig_rax);
        }
    }

    return 0;
}
