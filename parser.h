#ifndef PARSER_H
#define PARSER_H

struct parser {
    char data[20];
    int type;
};

struct parser *parsing(int size, char *string, int *count);

#endif