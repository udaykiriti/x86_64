#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// syscall table (x86_64)
const char *syscall_name(long num) {
    switch (num) {
        case 0: return "read";
        case 1: return "write";
        case 2: return "open";
        case 3: return "close";
        case 9: return "mmap";
        case 12: return "brk";
        case 21: return "access";
        case 59: return "execve";
        case 60: return "exit";
        case 231: return "exit_group";
        default: return "unknown";
    }
}

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execl("/bin/ls", "ls", NULL);
    } 
    else {
        int status;
        waitpid(pid, &status, 0);

        while (1) {
            ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
            waitpid(pid, &status, 0);

            if (WIFEXITED(status)) break;

            struct user_regs_struct regs;
            ptrace(PTRACE_GETREGS, pid, NULL, &regs);

            printf("Syscall: %s (%lld)\n",
                   syscall_name(regs.orig_rax),
                   regs.orig_rax);
        }
    }

    return 0;
}
