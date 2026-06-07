#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#define PERCENT 0
#define HTML 1
#define CHAR 2
#define LETTER 3

struct parser {
    char data[20];
    int type;
};



struct parser *parsing(int size, char *string, int *count) {

    struct parser *parsers = malloc(100 * sizeof(struct parser));

    int parserIndex = 0;

    for (char *p = string; *p != '\0'; p++) {

        char c = (char)*p;

        // // percent-encoding %
        if(c == '%') {
            char pEncoding[4];

            int num = 0;
            
            for (*p; p < string + size; p++) {
                char c = (char)*p;
                pEncoding[num] = *p;

                num++;

                if(num == 3) {
                   pEncoding[num] = '\0';

                    strcpy(parsers[parserIndex].data, pEncoding);
                    parsers[parserIndex].type = PERCENT;

                    parserIndex++;
                    break;
                }

            }

        // HTML entity &
        } else if(c == '&') {

            int num = 0;
            char htmlEntity[10];

            for (*p; p < string + size; p++) {

                char c = (char)*p;
                htmlEntity[num] = *p;

                num++;
                if(c == ';') {
                    htmlEntity[num] = '\0';

                    strcpy(parsers[parserIndex].data, htmlEntity);
                    parsers[parserIndex].type = HTML;

                    parserIndex++;
                    break;
                }

            }

        // char()
        } else if(*p == 'c' && *(p + 1) == 'h' && *(p + 2) == 'a' && *(p + 3) == 'r' && *(p + 4) == '(' || *p == 'C' && *(p + 1) == 'H' && *(p + 2) == 'A' && *(p + 3) == 'R' && *(p + 4) == '(' ) {
            int num = 0;

            char charSeq[10];

            for (*p; p < string + size; p++) {

                char c = (char)*p;
                charSeq[num] = *p;

                num++;
                if(c == ')') {
                    charSeq[num] = '\0';


                    strcpy(parsers[parserIndex].data, charSeq);
                    parsers[parserIndex].type = PERCENT;

                    parserIndex++;
                    break;
                }
            }
            
        // letter
        } else {
            char letter[2];
            letter[0] = *p;
            letter[1] = '\0';
            
            strcpy(parsers[parserIndex].data, letter);
            parsers[parserIndex].type = LETTER;

            parserIndex++;
        }
    }

    *count = parserIndex;
    return parsers;
}