#ifndef HASHMAP_C
#define HASHMAP_C

typedef struct Node {
  const char *key; 
  char *value;
  struct Node *next;
} Node;

typedef struct HashMap {
  int size, capacity;
  struct Node** arr;
} HashMap;

HashMap *hashmap_create(int capacity);

int hash_function(const char *string, int capacity);

void insert(HashMap *hashmap, const char *key, char *value);

char* get(HashMap *hashmap, const char *key);

void free_hashmap(HashMap *hashmap);

#endif



