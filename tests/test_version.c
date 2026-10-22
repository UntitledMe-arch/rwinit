#include <stdio.h>
#include <string.h>

#include <rwinit/version.h>

int
main(void)
{
    if (strcmp(RWINIT_NAME, "rwinit") != 0) {
        fprintf(stderr, "FAIL: unexpected project name\n");
        return 1;
    }

    if (strcmp(RWINIT_VERSION, "0.1.0") != 0) {
        fprintf(stderr, "FAIL: unexpected version\n");
        return 1;
    }

    puts("PASS: version constants");
    return 0;
}
