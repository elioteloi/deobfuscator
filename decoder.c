#include <stdlib.h>

#include "decoder.h"

/*
TOKENS TYPE!
0 - HTML entities
1 - HTML decimal
2 - HTML hexadecimal
3 - Char sequence
4 - Percent encouding
*/

char *decoder(char **encoded_text) {
    
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
