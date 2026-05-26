#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "parser.h"

#define SIZE 62

int main(void) {

    char line[SIZE] = "&copy;&trade;H%65&#108;l&#x6F;%20WCHAR(111)char(55)r&#108;d";

    int arrIndex;

    char **result = parsing(SIZE, line, &arrIndex);


    for(int i = 0; i < arrIndex; i++) {
        printf("the array: %s\n", result[i]);
    }

    for(int i = 0; i < arrIndex; i++) {
        free(result[i]);
    }
}