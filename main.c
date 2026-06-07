#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "parser.h"
#include "decoder.h"

#define SIZE 62

int main(void) {

    char line[SIZE] = "&copy;&trade;H%65&#108;l&#x6F;%20WCHAR(111)char(55)r&#108;d";

    int parserIndex;

    struct parser *result = parsing(SIZE, line, &parserIndex);


    for(int i = 0; i < parserIndex; i++) {
        printf("the array: %s, type: %d\n", result[i].data, result[i].type);
    }

    printf("length: %d\n", parserIndex);
    
    free(result);
}