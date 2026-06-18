#include <stdlib.h>
#include <string.h>

#include "decoder.h"
#include "hash-table.h"

/*
TOKENS TYPE!
HTML entities
HTML decimal
HTML hexadecimal
Char sequence
Percent encouding
*/

/*
Funcao auxiliar interna do decoder.c.

Ela adiciona UM caractere no final da string decodificada.
Recebe decoded_text como char ** porque a funcao pode precisar chamar realloc().
Quando realloc() muda o endereco da memoria, precisamos atualizar o ponteiro original
que esta dentro da funcao decoder().

Retorna 1 quando conseguiu adicionar o caractere.
Retorna 0 quando houve erro de alocacao de memoria.
*/
static int append_char(char **decoded_text, int *index, int *capacity, char character) {
    /*
    Precisamos de 2 espacos livres:
    1 para o caractere novo
    1 para o '\0', que marca o fim da string em C.
    */
    if (*index + 2 > *capacity) {
        int new_capacity = *capacity * 2;
        char *resized_text = realloc(*decoded_text, new_capacity);

        /*
        Se realloc falhar, ele retorna NULL.
        Nesse caso, avisamos o decoder para parar e liberar a memoria ja usada.
        */
        if (resized_text == NULL) {
            return 0;
        }

        /* Atualiza o ponteiro e a capacidade depois do crescimento da string. */
        *decoded_text = resized_text;
        *capacity = new_capacity;
    }

    /* Coloca o caractere na posicao atual e avanca o indice. */
    (*decoded_text)[*index] = character;
    (*index)++;

    /* Mantem a string sempre terminada com '\0'. */
    (*decoded_text)[*index] = '\0';

    return 1;
}

/*
Funcao auxiliar interna do decoder.c.

Ela adiciona uma string inteira no final da string decodificada.
Isso e necessario porque algumas HTML entities nomeadas nao cabem em apenas um char.
Por exemplo: &copy; vira "©" e &trade; vira "™". Em UTF-8 esses simbolos podem
ocupar mais de um byte, entao tratamos como string, nao como char.

Retorna 1 quando conseguiu adicionar o texto.
Retorna 0 quando houve erro de alocacao de memoria.
*/
static int append_string(char **decoded_text, int *index, int *capacity, char *text) {
    int text_length = strlen(text);

    /*
    Se a string atual nao tem espaco suficiente para receber text + '\0',
    aumentamos a capacidade ate caber tudo.
    */
    if (*index + text_length + 1 > *capacity) {
        int new_capacity = *capacity;

        /*
        Dobra a capacidade quantas vezes forem necessarias.
        Isso evita fazer varios realloc() pequenos em sequencia.
        */
        while (*index + text_length + 1 > new_capacity) {
            new_capacity *= 2;
        }

        char *resized_text = realloc(*decoded_text, new_capacity);

        /* Falha de memoria: o decoder vai interromper o processo com seguranca. */
        if (resized_text == NULL) {
            return 0;
        }

        *decoded_text = resized_text;
        *capacity = new_capacity;
    }

    /* Copia text para o final da string decodificada. */
    strcpy(*decoded_text + *index, text);

    /* Atualiza o indice para a proxima posicao livre. */
    *index += text_length;

    return 1;
}

/*
Funcao principal do decodificador.

Ela recebe:
- encoded_text: ponteiro para o primeiro token gerado pelo parser.
- count: quantidade de tokens existentes no vetor.

A funcao percorre token por token, identifica o tipo e chama a funcao correta
para decodificar. O resultado final e acumulado em decoded_text, que e retornado
para quem chamou a funcao.

Importante: quem chama decoder() fica responsavel por dar free() na string retornada.
*/
char *decoder(Token *encoded_text, int count) {
    /*
    Carrega a tabela hash das HTML entities nomeadas.
    Exemplo: &copy; -> "©"
    */
    load_entities();

    /*
    Comecamos com uma capacidade inicial de 64 bytes.
    Se a string final ficar maior, append_char() e append_string() aumentam
    esse espaco usando realloc().
    */
    int capacity = 64;
    int decoded_index = 0;
    char *decoded_text = malloc(capacity * sizeof(char));

    /* Se malloc falhar, liberamos a tabela hash antes de retornar erro. */
    if (decoded_text == NULL) {
        free_entities();
        return NULL;
    }

    /* Inicializa a string como vazia. */
    decoded_text[0] = '\0';

    /* Percorre todos os tokens criados pelo parser. */
    for (int i = 0; i < count; i++) {
        char *token = encoded_text[i].data;
        int token_type = encoded_text[i].type;
        int appended = 1;

        /*
        Percent encoding: tokens como "%65".
        A funcao decode_percent_encouding() transforma o valor hexadecimal em char.
        */
        if (token_type == PERCENT) {
            appended = append_char(&decoded_text, &decoded_index, &capacity, decode_percent_encouding(token));

        /*
        Char sequence: tokens como "char(65)" ou "CHAR(65)".
        A funcao decode_char_sequence() pega o numero dentro dos parenteses.
        */
        } else if (token_type == CHAR) {
            appended = append_char(&decoded_text, &decoded_index, &capacity, decode_char_sequence(token));

        /*
        Texto literal: letras/caracteres que nao estavam codificados.
        Nesse caso, apenas copiamos o token para a saida final.
        */
        } else if (token_type == LETTER) {
            appended = append_string(&decoded_text, &decoded_index, &capacity, token);

        /*
        Tokens HTML chegam todos com o mesmo tipo pelo parser.
        Por isso o decoder precisa diferenciar:
        - "&#x..." ou "&#X..." -> HTML hexadecimal
        - "&#..."              -> HTML decimal
        - "&copy;"             -> HTML entity nomeada
        */
        } else if (token_type == HTML) {
            if (strncmp(token, "&#x", 3) == 0 || strncmp(token, "&#X", 3) == 0) {
                appended = append_char(&decoded_text, &decoded_index, &capacity, decode_html_hexadecimal(token));
            } else if (strncmp(token, "&#", 2) == 0) {
                appended = append_char(&decoded_text, &decoded_index, &capacity, decode_html_decimal(token));
            } else {
                /* Busca a entity nomeada na tabela hash carregada no inicio. */
                char *entity = decode_html_entity(token);

                if (entity != NULL) {
                    appended = append_string(&decoded_text, &decoded_index, &capacity, entity);
                } else {
                    /*
                    Se a entity nao existir na tabela, mantemos o token original.
                    Isso evita perder informacao quando aparece uma entity desconhecida.
                    */
                    appended = append_string(&decoded_text, &decoded_index, &capacity, token);
                }
            }
        }

        /*
        Se qualquer append falhar, liberamos tudo que foi alocado antes de sair.
        Assim evitamos vazamento de memoria.
        */
        if (!appended) {
            free(decoded_text);
            free_entities();
            return NULL;
        }
    }

    /*
    A tabela hash so e necessaria durante a decodificacao.
    Depois que copied/append_string copiou os valores para decoded_text,
    podemos liberar a tabela com seguranca.
    */
    free_entities();

    /* Retorna a string final decodificada. */
    return decoded_text;
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
    Retornando assim um ponteiro pro primeiro endereco da entidade, a entidade nao e um character pois ocupa mais de um byte na memoria
    */
    return hash_get(token);
}
