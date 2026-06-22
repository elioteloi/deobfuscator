#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "decoder.h"

// COMANDO DE COMPILAÇÃO: gcc main.c parser.c decoder.c hash-table.c -o main

int main(void) {

    // pointer to access the file and read it
    FILE *fptr = fopen("filename.txt", "r");

    // find the end of the file to see size
    fseek(fptr, 0, SEEK_END);

    // add size to variable 
    long size = ftell(fptr);

    // put cursor to the beginning of the file
    rewind(fptr);


    // allocate array to the data and add plus for the end of the file
    char line[size + 1];

    // read the file and add it to the variable
    fread(line, 1, size, fptr);

    // close the memory address resource
    fclose(fptr);


    

    int parserIndex;

    Token *result = parsing(size, line, &parserIndex);


    for(int i = 0; i < parserIndex; i++) {
        printf("the array: %s, type: %d\n", result[i].data, result[i].type);
    }

    printf("length: %d\n", parserIndex);

    char *decodedText = decoder(result, parserIndex);

    if(decodedText != NULL) {
        printf("decoded: %s\n", decodedText);

        FILE *fptr;

        // Open a file to write it
        fptr = fopen("filename.txt", "w");

        // Write some text to the file
        fputs(decodedText, fptr);

        // close the memory address resource
        fclose(fptr);

        free(decodedText);
    }
    
    free(result);
}

// COMANDO DE COMPILAÇÃO: gcc main.c parser.c decoder.c hash-table.c -o main
