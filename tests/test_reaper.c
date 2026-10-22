#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include <rwinit/core.h>

int
main(void)
{
    pid_t child = fork();

    if (child < 0) {
        perror("fork");
        return 1;
    }

    if (child == 0) {
        _exit(0);
    }

    /*
     * Give the child time to exit, leaving a zombie
     * for the reaper to collect.
     */
    struct timespec delay = {
        .tv_sec = 0,
        .tv_nsec = 100000000
    };

    while (nanosleep(&delay, &delay) < 0) {
        if (errno != EINTR) {
            perror("nanosleep");
            return 1;
        }
    }

    int reaped = rwinit_reap_children();

    if (reaped != 1) {
        fprintf(stderr, "FAIL: expected 1 reaped child, got %d\n",
                reaped);
        return 1;
    }

    /*
     * Verify the child is no longer waitable.
     */
    int status;
    if (waitpid(child, &status, WNOHANG) != -1) {
        fprintf(stderr, "FAIL: child was not reaped\n");
        return 1;
    }

    puts("PASS: child reaping");
    return 0;
}
