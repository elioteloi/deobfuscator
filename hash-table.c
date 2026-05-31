#include <stdlib.h>
#include <string.h>

#include "hash-table.h"

#define TABLE_SIZE 1024

typedef struct HashEntry_ {
    char *key;
    char *value;
    struct HashEntry_ *next;
} HashEntry;

HashEntry *hash_table[TABLE_SIZE];

unsigned long djb2(const char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

int hash_index(const char *key) {
    return djb2(key) % TABLE_SIZE;
}

void hash_insert(char *key, char *value) {
    int index = hash_index(key);

    HashEntry *entry = malloc(sizeof(HashEntry));

    entry->key = key;
    entry->value = value;

    entry->next = hash_table[index];

    hash_table[index] = entry;
}

char *hash_get(char *key) {
    int index = hash_index(key);

    HashEntry *current = hash_table[index];

    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            return current->value;
        }

        current = current->next;
    }

    return NULL;
}

void load_entities() {
    /* Caracteres reservados HTML */
    hash_insert("&amp;", "&");
    hash_insert("&lt;", "<");
    hash_insert("&gt;", ">");
    hash_insert("&quot;", "\"");
    hash_insert("&apos;", "'");

    /* Espaços */
    hash_insert("&nbsp;", " ");

    /* Símbolos comuns */
    hash_insert("&copy;", "©");
    hash_insert("&reg;", "®");
    hash_insert("&trade;", "™");

    /* Moedas */
    hash_insert("&euro;", "€");
    hash_insert("&pound;", "£");
    hash_insert("&yen;", "¥");
    hash_insert("&cent;", "¢");

    /* Matemática */
    hash_insert("&plusmn;", "±");
    hash_insert("&times;", "×");
    hash_insert("&divide;", "÷");
    hash_insert("&ne;", "≠");
    hash_insert("&le;", "≤");
    hash_insert("&ge;", "≥");

    /* Setas */
    hash_insert("&larr;", "←");
    hash_insert("&uarr;", "↑");
    hash_insert("&rarr;", "→");
    hash_insert("&darr;", "↓");

    /* Grego */
    hash_insert("&alpha;", "α");
    hash_insert("&beta;", "β");
    hash_insert("&gamma;", "γ");
    hash_insert("&delta;", "δ");
    hash_insert("&pi;", "π");
    hash_insert("&omega;", "ω");
}

void free_entities() {
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        HashEntry *current = hash_table[i];

        while (current != NULL)
        {
            HashEntry *next = current->next;

            free(current);

            current = next;
        }

        hash_table[i] = NULL;
    }
}