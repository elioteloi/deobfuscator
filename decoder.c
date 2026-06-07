#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

char **decoder(int size, char **string, int *count) {
    for(int i = 0; i < *count; i++) {
        printf("data: %s\n", string[i]);
    }
}
