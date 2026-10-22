
#include <stdio.h>
#include <string.h>

#include <rwinit/core.h>
#include <rwinit/version.h>

static void
print_usage(const char *program)
{
    fprintf(stderr,
            "Usage: %s [--version | --help | --run]\n",
            program);
}

int
main(int argc, char **argv)
{
    if (argc == 2 && strcmp(argv[1], "--version") == 0) {
        printf("%s %s\n", RWINIT_NAME, RWINIT_VERSION);
        return 0;
    }

    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        printf("%s - Linux init manager\n", RWINIT_NAME);
        print_usage(argv[0]);
        return 0;
    }

    if (argc == 2 && strcmp(argv[1], "--run") == 0) {
        fprintf(stderr, "%s: starting development event loop\n",
                RWINIT_NAME);

        if (rwinit_run() < 0) {
            perror("rwinit");
            return 1;
        }

        fprintf(stderr, "%s: event loop stopped\n", RWINIT_NAME);
        return 0;
    }

    if (argc != 1) {
        print_usage(argv[0]);
        return 2;
    }

    printf("%s %s: project skeleton ready\n",
           RWINIT_NAME, RWINIT_VERSION);

    return 0;
}
