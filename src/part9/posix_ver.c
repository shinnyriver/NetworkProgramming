#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    long version = sysconf(_SC_VERSION);
    printf("Posix version: %ld\n", version);
    if(version >= 199506L) {
        printf("POSIX.1c is supported\n");
    } else {
        printf("POSIX.1c is not supported\n");
    }
    return 0;
}