#include <stdio.h>
#include "utils/hashmap.h"
#include "config.h"

int main(int argc, char *argv[]){
  if (argc < 3) {
    printf("Not enough arguments.\n");
    return 1;
  }
  const char *program_name = argv[0];
  const char *command = argv[1];
  const char *group = argv[2];
  printf("command: %s\n", command);
  printf("group: %s\n", group);

  HashMap *hashmap = hashmap_create(10);

  load_config(hashmap);
  char *test_key = "cu-scraper";
  char *values = get(hashmap, test_key);
  printf("Values: %s", values);

  free_hashmap(hashmap);

  return 0;
}

