#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) {
    printf("\nCaught signal %d (Ctrl+C). Not exiting!\n", sig);
}

int main() {
    struct sigaction sa;

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    // Register handler for SIGINT (Ctrl+C)
    sigaction(SIGINT, &sa, NULL);

    printf("Running... Press Ctrl+C\n");

    while (1) {
        sleep(1);
        printf("Still alive...\n");
    }

    return 0;
}
