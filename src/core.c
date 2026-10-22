
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <rwinit/core.h>

int
rwinit_reap_children(void)
{
    int reaped = 0;

    for (;;) {
        int status;
        pid_t pid = waitpid(-1, &status, WNOHANG);

        if (pid > 0) {
            ++reaped;
            continue;
        }

        if (pid == 0 || (pid < 0 && errno == ECHILD)) {
            return reaped;
        }

        if (pid < 0 && errno == EINTR) {
            continue;
        }

        return -1;
    }
}

int
rwinit_run(char *const argv[])
{
    if (argv == NULL || argv[0] == NULL) {
        errno = EINVAL;
        return -1;
    }

    /*
     * Until PID 1 shutdown behavior is implemented,
     * never risk exiting as the system's init process.
     */
    if (getpid() == 1) {
        errno = EPERM;
        return -1;
    }

    pid_t child = fork();

    if (child < 0) {
        return -1;
    }

    if (child == 0) {
        execvp(argv[0], argv);

        /* Reached only if execvp fails. */
        _exit(errno == ENOENT ? 127 : 126);
    }

    int status;

    for (;;) {
        pid_t result = waitpid(child, &status, 0);

        if (result == child) {
            break;
        }

        if (result < 0 && errno == EINTR) {
            continue;
        }

        return -1;
    }

    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }

    if (WIFSIGNALED(status)) {
        return 128 + WTERMSIG(status);
    }

    return 1;
}
