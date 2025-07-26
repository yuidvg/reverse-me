#include <stdio.h>
#include <string.h>

int main(void) {
    const char *const password = "__stack_check"; // expected key
    char input[256] = {0};

    printf("Please enter key: ");
    /* Read at most sizeof(input)-1 characters to avoid overflow */
    if (scanf("%255s", input) != 1) {
        return 1;
    }

    if (strcmp(input, password) == 0) {
        puts("Good job.");
    } else {
        puts("Nope.");
    }

    return 0;
} 