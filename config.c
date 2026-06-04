#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "utils/hashmap.h"

void load_config(HashMap *hashmap){
  FILE *fptr;

  fptr = fopen("/etc/pmgr/config.ini", "r");
  if (fptr == NULL) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }
  char line[256];
  int length = 0;
  char *current_group_ptr = NULL;
  char *units_value_ptr = NULL;
  while(fgets(line, 256, fptr)) {
    char *trimmed = line;
    while (*trimmed == ' ') trimmed++;
    if (trimmed[0] == '['){
      char *after_bracket = trimmed + 1;
      char *closing_bracket = strchr(after_bracket, ']');

      if (closing_bracket == NULL) exit(EXIT_FAILURE);

      size_t group_length = closing_bracket - after_bracket;

      char current_group[group_length + 1];
      strncpy(current_group, after_bracket, group_length);

      current_group[group_length] = '\0';

      current_group_ptr = malloc(group_length + 1);
      strcpy(current_group_ptr, current_group);
    } else if (trimmed[0] == 'u') {
      char *after_equal = strchr(trimmed, '=') + 1; 
      while (*after_equal == ' ') after_equal++;

      char *end = strchr(after_equal, '\n');

      size_t units_value_length = end - after_equal;

      char units_value[units_value_length + 1];
      strncpy(units_value, after_equal, units_value_length);

      units_value[units_value_length] = '\0';

      units_value_ptr = malloc(units_value_length + 1);
      strcpy(units_value_ptr, units_value);

      insert(hashmap, current_group_ptr, units_value_ptr); 
    } else if (trimmed[0] == '\n') {
      continue;
    } else {
      printf("Format error in config file on line: %d\n", length+1);
    }
    length++;
  }
  fclose(fptr);
}

