
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <rwinit/core.h>
#include <rwinit/version.h>

static void
print_usage(const char *program)
{
    fprintf(stderr,
            "Usage:\n"
            "  %s --version\n"
            "  %s --help\n"
            "  %s -- PROGRAM [ARGS...]\n",
            program, program, program);
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

    if (argc >= 3 && strcmp(argv[1], "--") == 0) {
        int status = rwinit_run(&argv[2]);

        if (status < 0) {
            perror("rwinit");
            return 1;
        }

        return status;
    }

    print_usage(argv[0]);
    return 2;
}
