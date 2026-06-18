#ifndef DECODER_H
#define DECODER_H

#include "parser.h"

char *decoder(Token *encoded_text, int count);

char decode_html_hexadecimal(char *token);

char decode_percent_encouding(char *token);

char decode_html_decimal(char *token);

char decode_char_sequence(char *token);

char *decode_html_entity(char *token);

#endif
