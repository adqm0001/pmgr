#ifndef HASHMAP_C
#define HASHMAP_C

typedef struct Node {
  char *key; 
  char *value;
  struct Node *next;
} Node;

typedef struct HashMap {
  int size, capacity;
  struct Node** arr;
} HashMap;

HashMap *hashmap_create(int capacity);

int hash_function(char *string, int capacity);

void insert(HashMap *hashmap, char *key, char *value);

char* get(HashMap *hashmap, char *key);

void free_hashmap(HashMap *hashmap);

#endif



