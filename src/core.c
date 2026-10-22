
#define _GNU_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <rwinit/core.h>

static int signal_pipe[2] = {-1, -1};
static volatile sig_atomic_t stop_requested = 0;

static void
signal_handler(int signo)
{
    int saved_errno = errno;
    char event = 'C';

    if (signo == SIGTERM || signo == SIGINT) {
        stop_requested = 1;
        event = 'S';
    }

    if (signal_pipe[1] >= 0) {
        (void) write(signal_pipe[1], &event, 1);
    }

    errno = saved_errno;
}

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

static int
install_signal_handlers(void)
{
    struct sigaction action = {0};

    action.sa_handler = signal_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    if (sigaction(SIGCHLD, &action, NULL) < 0 ||
        sigaction(SIGTERM, &action, NULL) < 0 ||
        sigaction(SIGINT, &action, NULL) < 0) {
        return -1;
    }

    return 0;
}

int
rwinit_run(void)
{
    struct pollfd pfd;

    stop_requested = 0;

    if (pipe2(signal_pipe, O_NONBLOCK | O_CLOEXEC) < 0) {
        return -1;
    }

    if (install_signal_handlers() < 0) {
        close(signal_pipe[0]);
        close(signal_pipe[1]);
        signal_pipe[0] = signal_pipe[1] = -1;
        return -1;
    }

    pfd.fd = signal_pipe[0];
    pfd.events = POLLIN;
    pfd.revents = 0;

    while (!stop_requested) {
        int result = poll(&pfd, 1, -1);

        if (result < 0 && errno == EINTR) {
            continue;
        }

        if (result < 0) {
            break;
        }

        if (pfd.revents & (POLLERR | POLLHUP | POLLNVAL)) {
            errno = EIO;
            break;
        }

        if (pfd.revents & POLLIN) {
            char buffer[128];

            while (read(signal_pipe[0], buffer, sizeof(buffer)) > 0) {
                /* Drain the notification pipe. */
            }
        }

        if (rwinit_reap_children() < 0) {
            break;
        }
    }

    close(signal_pipe[0]);
    close(signal_pipe[1]);
    signal_pipe[0] = signal_pipe[1] = -1;

    return stop_requested ? 0 : -1;
}
