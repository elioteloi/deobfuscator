#ifndef PARSER_H
#define PARSER_H

#define PERCENT 0
#define HTML 1
#define CHAR 2
#define LETTER 3

typedef struct parser {
    char data[20];
    int type;
} Token;

Token *parsing(int size, char *string, int *count);

#endif