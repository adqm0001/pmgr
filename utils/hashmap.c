#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "hashmap.h"

HashMap *hashmap_create(int capacity){
  HashMap *hashmap = malloc(sizeof(HashMap));
  Node** array = malloc(sizeof(Node*)*capacity);
  for (int i = 0; i < capacity; i++){
    *(&array[0] + i) = NULL;
  }
  hashmap->capacity = capacity;
  hashmap->size = 0;
  hashmap->arr = array;

  return hashmap;
}

int hash_function(char *string, int capacity){
  uint64_t hash = 14695981039346656037ULL;
  for (int i = 0; i < strlen(string); i++) {
    hash = hash ^ string[i];
    hash = hash * 1099511628211;
  }
  return (int)(hash % capacity);
}

void insert(HashMap *hashmap, char *key, char *value){
  int index = hash_function(key, hashmap->capacity);
  Node **array = hashmap->arr;
  Node *to_add = malloc(sizeof(Node));
  to_add->key = key;
  to_add->value = value;
  to_add->next = array[index];
  array[index] = to_add;
  hashmap->size = hashmap->size + 1;
}

char* get(HashMap *hashmap, char *key){
  int index = hash_function(key, hashmap->capacity);
  Node **array = hashmap->arr;
  Node *head = array[index];
  while (head != NULL){
    if (!(strcmp(head->key, key))) return head->value;
    head = head->next;
  }
  return NULL;
}

void free_hashmap(HashMap *hashmap){
  Node **array = hashmap->arr;
  for (int i = 0; i < hashmap->capacity; i++){
    Node *head = array[i];
    while (head != NULL){
      Node *temp = head;
      head = head->next;
      free(temp);
    }
  }
  free(array);
  free(hashmap);
}
