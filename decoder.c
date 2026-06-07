<<<<<<< HEAD
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

char **decoder(int size, char **string, int *count) {
    for(int i = 0; i < *count; i++) {
        printf("data: %s\n", string[i]);
    }
=======
#include <stdlib.h>

#include "decoder.h"
#include "hash-table.h"

/*
TOKENS TYPE!
0 - HTML entities
1 - HTML decimal
2 - HTML hexadecimal
3 - Char sequence
4 - Percent encouding
*/

char *decoder(char **encoded_text) {
    load_entities();

    /*
    Implementação do decodificador
    */

    free_entities();
}

char decode_html_hexadecimal(char *token) { // Decodifica tokens do tipo HTML HEXADECIMAL: &#x26;
    long value = strtol(token + 3, NULL, 16);

    return (char) value;
}

char decode_percent_encouding(char *token) { // Decodifica tokens do tipo PERCENT ENCOUDING: %26
    long value = strtol(token + 1, NULL, 16);

    return (char) value;
}

char decode_html_decimal(char *token) { // Decodifica tokens do tipo HTML DECIMAL: &#65;
    long value = strtol(token + 2, NULL, 10);

    return (char) value;
}

char decode_char_sequence(char *token) { // Decodifica tokens do tipo CHAR SEQUENCE: char(65)
    long value = strtol(token + 5, NULL, 10);

    return (char) value;
}

char *decode_html_entity(char *token) { // Decodifica tokens do tipo HTML ENTITY baseado na tabela hash: &copy;
    /*
    Retornando assim um ponteiro pro primeiro endereço da entidade, a entidade não é um character pois ocupa mais de um byte na memória
    */
    return hash_get(token);
>>>>>>> ae87aded6e04df0a0d9493637c8600fbb82e591b
}
