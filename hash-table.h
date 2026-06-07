typedef struct HashEntry_ HashEntry;

unsigned long djb2(const char *str);

int hash_index(const char *key);

void hash_insert(char *key, char *value);

char *hash_get(char *key);

void load_entities();

void free_entities();