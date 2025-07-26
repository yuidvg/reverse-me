#include <stdio.h>
#include <string.h>

/*
 * Replica of the original level2 crackme.
 * Expected user input is a decimal-encoded string which, once decoded
 * (see README.md), yields the word "delabere". We short-circuit
 * the decoding and simply compare against the pre-computed
 * canonical key to keep this sample concise.
 */

int main(void) {
    const char *const key = "00101108097098101114101"; /* 23 chars */
    char buf[32] = {0};

    printf("Please enter key: ");
    fflush(stdout);

    if (scanf("%23s", buf) != 1) {
        return 1;
    }

    if (strcmp(buf, key) == 0) {
        puts("Good job.");
    } else {
        puts("Nope.");
    }

    return 0;
} 