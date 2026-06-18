#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "decoder.h"

// COMANDO DE COMPILAÇÃO: gcc main.c parser.c decoder.c hash-table.c -o main

int main(void) {

    char line[] = "&copy;%20The&#32;quick&#x20;brown%20fox%20CHAR(106)umps&#32;over%20the&#x20;lazy%20char(100)og&trade;%20&plusmn;%20done&#33;";
    
    int length = sizeof(line) - 1;

    int parserIndex;

    Token *result = parsing(length, line, &parserIndex);


    for(int i = 0; i < parserIndex; i++) {
        printf("the array: %s, type: %d\n", result[i].data, result[i].type);
    }

    printf("length: %d\n", parserIndex);

    char *decodedText = decoder(result, parserIndex);

    if(decodedText != NULL) {
        printf("decoded: %s\n", decodedText);
        free(decodedText);
    }
    
    free(result);
}

// COMANDO DE COMPILAÇÃO: gcc main.c parser.c decoder.c hash-table.c -o main
