#include <errno.h>
#include <linux/futex.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

static atomic_int word = 0;

static int wait(atomic_int *addr, int want) {
    return syscall(
        SYS_futex,
        (int *)addr,
        FUTEX_WAIT,
        want,
        NULL,
        NULL,
        0
    );
}

static int wake(atomic_int *addr, int num) {
    return syscall(
        SYS_futex,
        (int *)addr,
        FUTEX_WAKE,
        num,
        NULL,
        NULL,
        0
    );
}

static void *work(void *arg) {
    (void)arg;

    puts("worker: waiting for main thread");

    while (atomic_load(&word) == 0) {
        if (wait(&word, 0) == -1 && errno != EAGAIN) {
            perror("futex wait");
            return NULL;
        }
    }

    puts("worker: woke up and saw word = 1");
    return NULL;
}

int main(void) {
    pthread_t tid;

    if (pthread_create(&tid, NULL, work, NULL) != 0) {
        perror("pthread_create");
        return 1;
    }

    sleep(1);

    puts("main: setting word and waking worker");
    atomic_store(&word, 1);

    if (wake(&word, 1) == -1) {
        perror("futex wake");
        return 1;
    }

    if (pthread_join(tid, NULL) != 0) {
        perror("pthread_join");
        return 1;
    }

    puts("done");
    return 0;
}
