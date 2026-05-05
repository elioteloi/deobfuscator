#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define size 50

int main(void) {

    char string[size] = "H%65&#108;l&#x6F;%20WCHAR(111)char(55)r&#108;d";

    // char string[size] = "char(65)char(22)";

    // char string[size] = "s%3Fn&#32;%65e";

    char *arr[size];

    int arrIndex = 0;

    printf("############### %s\n", string);

    for (char *p = string; *p != '\0'; p++) {

        char c = (char)*p;

        // // percent-encoding %
        if(c == '%') {
            printf("character percent-encoding:%c %p\n", c, p);

            char pEncoding[4];

            int num = 0;
            
            for (*p; p < string + size; p++) {
                char c = (char)*p;
                pEncoding[num] = *p;
                printf("character percent-encoding: %c %p\n", c, p);

                num++;

                if(num == 3) {
                    printf("break %p\n", p);
                    pEncoding[num] = '\0';
                    arr[arrIndex] = malloc(10);
                    strcpy(arr[arrIndex], pEncoding);
                    arrIndex++;
                    printf("percent-encoding:  %s\n", pEncoding);
                    break;
                }

            }

        // HTML entity &
        } else if(c == '&') {

            int num = 0;
            char htmlEntity[10];

            for (*p; p < string + size; p++) {

                char c = (char)*p;
                printf("character HTML Entity: %c %p\n", c, p);
                htmlEntity[num] = *p;

                num++;
                if(c == ';') {
                    printf("break %p\n", p);
                    htmlEntity[num] = '\0';
                    arr[arrIndex] = malloc(10);
                    strcpy(arr[arrIndex], htmlEntity);
                    arrIndex++;
                    printf("htmlEntity:  %s\n", htmlEntity);
                    break;
                }

            }

        // char()
        } else if(*p == 'c' && *(p + 1) == 'h' && *(p + 2) == 'a' && *(p + 3) == 'r' && *(p + 4) == '(' || *p == 'C' && *(p + 1) == 'H' && *(p + 2) == 'A' && *(p + 3) == 'R' && *(p + 4) == '(' ) {
            int num = 0;

            char charSeq[10];

            for (*p; p < string + size; p++) {

                char c = (char)*p;
                printf("character char(): %c %p\n", c, p);
                charSeq[num] = *p;

                num++;
                if(c == ')') {
                    printf("break %p\n", p);

                    charSeq[num] = '\0';
                    arr[arrIndex] = malloc(10);
                    strcpy(arr[arrIndex], charSeq);
                    arrIndex++;
                    
                    printf("char():  %s\n", charSeq);
                    break;
                }
            }
            
        // letter
        } else {
            printf("character letter: %c %p\n", c, p);
            char letter[2];
            letter[0] = *p;
            letter[1] = '\0';
            arr[arrIndex] = malloc(10);
            strcpy(arr[arrIndex], letter);

            arrIndex++;
            printf("letter:  %s\n", letter);

        }
    }

    printf("--------------------------------------\n");
    for(int i = 0; i < arrIndex; i++) {
        printf("the array: %s\n", arr[i]);
    }

    for(int i = 0; i < arrIndex; i++) {
        free(arr[i]);
    }

}