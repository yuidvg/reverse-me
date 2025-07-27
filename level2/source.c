#include <stdio.h>
#include <string.h>
#include <stdlib.h>          // ← atoi 用を追加

/*
 * Replica of the original level2 crackme.
 * Expected user input is a decimal-encoded string which, once decoded
 * (see README.md), yields the word "delabere". We short-circuit
 * the decoding and simply compare against the pre-computed
 * canonical key to keep this sample concise.
 */

int main(void) {
    char buf[32] = {0};          /* ユーザ入力最大 23 文字 + α */

    printf("Please enter key: ");
    fflush(stdout);

    if (scanf("%23s", buf) != 1) {
        puts("Nope.");
        return 0;
    }

    if (buf[0] != '0' || buf[1] != '0') {
        puts("Nope.");
        return 0;
    }

    char decoded[9];            
    decoded[0] = 'd';
    size_t dlen = 1;
    size_t pos  = 2;

    while (dlen < 8) {
        if (buf[pos] == '\0' || buf[pos + 1] == '\0' || buf[pos + 2] == '\0') {
            puts("Nope.");
            return 0;
        }

        char triplet[4] = { buf[pos], buf[pos + 1], buf[pos + 2], '\0' };
        int  val        = atoi(triplet);

        decoded[dlen++] = (char)val;
        pos += 3;
    }
    decoded[dlen] = '\0';

    if (strcmp(decoded, "delabere") == 0) {
        puts("Good job.");
    } else {
        puts("Nope.");
    }

    return 0;
} 