#include "utils/hashmap.h"
#include <string.h>
#include <stdio.h> 
#include <stdlib.h>

char **get_units(HashMap *hashmap, const char *key, int *count){
  char **units_arr = NULL;
  char *current_unit_ptr = NULL;
  *count = 0;
  char *values = get(hashmap, key);
  char delimiter = ',';
  int start_idx = 0;
  int end_idx = 0;
  int i = 0;

  for (i = 0; values[i] != '\0'; i++){
    if (values[i] == delimiter) {
      end_idx = i;
      if (end_idx - start_idx == 0) continue; 

      char current_unit[end_idx - start_idx + 1];
      for (int j = start_idx, unit_idx = 0; j < end_idx; j++, unit_idx++){
        current_unit[unit_idx] = values[j];
      }
      current_unit[end_idx - start_idx] = '\0';

      current_unit_ptr = malloc(end_idx - start_idx + 1);
      strcpy(current_unit_ptr, current_unit);

      units_arr = realloc(units_arr, (*count + 1) * sizeof(char *));
      units_arr[*count] = current_unit_ptr;
      (*count)++;

      end_idx = i + 1;
      start_idx = i + 1;
      while (values[start_idx] == ' ') start_idx++;
    }
  }
  end_idx = i;
  if (end_idx - start_idx == 0) return units_arr; 

  if (values[end_idx - 1] == '\n') end_idx--;

  char current_unit[end_idx - start_idx + 1];
  for (int j = start_idx, unit_idx = 0; j < end_idx; j++, unit_idx++){
    current_unit[unit_idx] = values[j];
  }
  current_unit[end_idx - start_idx] = '\0';

  current_unit_ptr = malloc(end_idx - start_idx + 1);
  strcpy(current_unit_ptr, current_unit);

  units_arr = realloc(units_arr, (*count + 1) * sizeof(char *));
  units_arr[*count] = current_unit_ptr;
  (*count)++;

  return units_arr;
}

void cmd_status(HashMap *hashmap, const char *key){
  int count = 0;
  char **units = get_units(hashmap, key, &count);
  for (int i = 0; i < count; i++){
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "systemctl status %s", units[i]);
    FILE *output = popen(cmd, "r"); 
    if (output == NULL) return;
    char line[256];
    while(fgets(line, 256, output)) {
      printf("%s", line);
    }
    pclose(output);
  }
}
void cmd_logs(HashMap *hashmap, const char *key){
  int count = 0;
  char **units = get_units(hashmap, key, &count);
  for (int i = 0; i < count; i++){
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "journalctl -u %s", units[i]);
    FILE *output = popen(cmd, "r"); 
    if (output == NULL) return;
    char line[256];
    while(fgets(line, 256, output)) {
      printf("%s", line);
    }
    pclose(output);
  }
}

void cmd_restart(HashMap *hashmap, const char *key){
  int count = 0;
  char **units = get_units(hashmap, key, &count);
  for (int i = 0; i < count; i++){
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "systemctl restart %s", units[i]);
    FILE *output = popen(cmd, "r"); 
    printf("Restarted %s\n", units[i]);
    pclose(output);
  }
}
