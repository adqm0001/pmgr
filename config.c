#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "utils/hashmap.h"

void load_config(HashMap *units, HashMap *commands){
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
  char *commands_value_ptr = NULL;
  char *command_name_ptr = NULL;
  int in_commands_section = 0;

  while(fgets(line, 256, fptr)) {
    char *trimmed = line;
    while (*trimmed == ' ') trimmed++;
    if (trimmed[0] == '['){
      in_commands_section = 0;
      char *after_bracket = trimmed + 1;
      char *closing_char = strchr(after_bracket, ']');
      if (strchr(after_bracket, ':') != NULL){
        in_commands_section = 1;
        closing_char = strchr(after_bracket, ':');
      }
      if (closing_char == NULL) exit(EXIT_FAILURE);

      size_t group_length = closing_char - after_bracket;

      char current_group[group_length + 1];
      strncpy(current_group, after_bracket, group_length);

      current_group[group_length] = '\0';
      free(current_group_ptr);
      current_group_ptr = malloc(group_length + 1);
      strcpy(current_group_ptr, current_group);
    } else if (in_commands_section){
      char *equal = strchr(trimmed, '=');
      char *end = strchr(trimmed, '\n');
      while (*(equal - 1) == ' ') equal--;
      size_t command_name_length = equal - trimmed;
      char command_name[command_name_length + 1];
      strncpy(command_name, trimmed, command_name_length);
      command_name[command_name_length] = '\0';
      command_name_ptr = malloc(command_name_length + 1);
      strcpy(command_name_ptr, command_name); 
      char *value_start = strchr(trimmed, '=') + 1; 
      while (*value_start == ' ') value_start++;
      size_t commands_value_length = end - value_start;
      char commands_value[commands_value_length + 1];
      strncpy(commands_value, value_start, commands_value_length);
      commands_value[commands_value_length] = '\0';
      commands_value_ptr = malloc(commands_value_length + 1);
      strcpy(commands_value_ptr, commands_value);
      size_t key_len = strlen(current_group_ptr) + strlen(command_name) + 2;
      char *key = malloc(key_len);
      snprintf(key, key_len, "%s:%s", current_group_ptr, command_name);
      insert(commands, key, commands_value_ptr); // Inserts (ex: key{cu-scraper:deploy},value{python3 r | Does something})
      char *existing = get(commands, current_group_ptr);
      if (existing != NULL) {
          size_t new_len = strlen(existing) + strlen(command_name_ptr) + 2;
          char *combined = malloc(new_len);
          snprintf(combined, new_len, "%s,%s", existing, command_name_ptr);
          free(existing);
          insert(commands, current_group_ptr, combined); // Inserts (ex: key{cu-scraper},value{deploy,test})

      } else {
          insert(commands, current_group_ptr, command_name_ptr); // Inserts (ex: key{cu-scraper}, value{deploy})
      }
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

      insert(units, current_group_ptr, units_value_ptr); 
    } else if (trimmed[0] == '\n') {
      continue;
    } else {
      printf("Format error in config file on line: %d\n", length+1);
    }
    length++;
  } fclose(fptr); 
}
