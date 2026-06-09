#include "utils/hashmap.h"
#include <string.h>
#include <stdio.h> 
#include <stdlib.h>

char **get_units(HashMap *units, const char *key, int *count){
  char **units_arr = NULL;
  char *current_unit_ptr = NULL;
  *count = 0;
  char *values = get(units, key);
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

char **get_custom_commands(HashMap *commands, const char *key, int *count){
  char **commands_info_arr = NULL;
  char *current_command_ptr = NULL;
  *count = 0;
  char *values = get(commands, key);
  char delimiter = ',';
  int start_idx = 0;
  int end_idx = 0;
  int i = 0;

  for (i = 0; values[i] != '\0'; i++){
    if (values[i] == delimiter){
      end_idx = i; 
      if (end_idx - start_idx == 0) continue;
      
      char current_command[end_idx - start_idx + 1];
      for (int j = start_idx, command_idx = 0; j < end_idx; j++, command_idx++){
        current_command[command_idx] = values[j];
      }
      current_command[end_idx - start_idx] = '\0';

      current_command_ptr = malloc(end_idx - start_idx + 1);
      strcpy(current_command_ptr, current_command);

      commands_info_arr = realloc(commands_info_arr, (*count + 1) * sizeof(char *));
      commands_info_arr[*count] = current_command_ptr;
      (*count)++;
      
      end_idx = i + 1;
      start_idx = i + 1;
      while (values[start_idx] == ' ') start_idx++;
    }
  }
  end_idx = i;
  if (end_idx - start_idx == 0) return commands_info_arr;
  
  char current_command[end_idx - start_idx + 1];
  for (int j = start_idx, command_idx = 0; j < end_idx; j++, command_idx++){
    current_command[command_idx] = values[j];
  }
  current_command[end_idx - start_idx] = '\0';

  current_command_ptr = malloc(end_idx - start_idx + 1);
  strcpy(current_command_ptr, current_command);

  commands_info_arr = realloc(commands_info_arr, (*count + 1) * sizeof(char *));
  commands_info_arr[*count] = current_command_ptr;
  (*count)++;

  return commands_info_arr;
}

char *get_command_value(HashMap *commands, const char *group, const char *command_name){
  char command_value[256];
  snprintf(command_value, sizeof(command_value), "%s:%s", group, command_name);
  
  return get(commands, command_value);
}

void cmd_status(HashMap *units, const char *key){
  int count = 0;
  char **units_arr = get_units(units, key, &count);
  for (int i = 0; i < count; i++){
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "systemctl status %s", units_arr[i]);
    printf("=== %s ===\n", units_arr[i]);
    FILE *output = popen(cmd, "r"); 
    if (output == NULL) return;
    char line[256];
    while(fgets(line, 256, output)) {
      printf("%s", line);
    }
    pclose(output);
  }
}
void cmd_logs(HashMap *units, const char *key, int max_logs){
  int count = 0;
  char **units_arr = get_units(units, key, &count);
  for (int i = 0; i < count; i++){
    char cmd[256];
    if (!max_logs) snprintf(cmd, sizeof(cmd), "journalctl -u %s", units_arr[i]);
    else snprintf(cmd, sizeof(cmd), "journalctl -u %s -n %d", units_arr[i], max_logs);
    printf("=== %s ===\n", units_arr[i]);
    FILE *output = popen(cmd, "r"); 
    if (output == NULL) return;
    char line[256];
    while(fgets(line, 256, output)) {
      printf("%s", line);
    }
    pclose(output);
  }
}

void cmd_restart(HashMap *units, const char *key){
  int count = 0;
  char **units_arr = get_units(units, key, &count);
  for (int i = 0; i < count; i++){
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "systemctl restart %s", units_arr[i]);
    printf("=== %s ===\n", units_arr[i]);
    FILE *output = popen(cmd, "r"); 
    printf("Restarted %s\n", units_arr[i]);
    pclose(output);
  }
}

void cmd_custom(HashMap *commands, const char *key){
  int count = 0;
  char **commands_arr = get_custom_commands(commands, key, &count);
  printf("Custom commands for %s:\n", key);
  for (int i = 0; i < count; i++){
    printf("  %s\n", commands_arr[i]);
    char *value = get_command_value(commands, key, commands_arr[i]);
    char *del = strchr(value, '|');
    if (del != NULL){
      char *run_end = del;
      while (run_end > value && *(run_end - 1) == ' ') run_end--;
      char *desc = del + 1;
      while (*desc == ' ') desc++;
      printf("    run:  %.*s\n", (int)(run_end - value), value);
      printf("    desc: %s\n", desc);
    } else {
      printf("    run:  %s\n", value);
    }
    printf("\n");
  }
}

void cmd_run(HashMap *commands, const char *group, const char *command){
  char *value = get_command_value(commands, group, command);
  if (value == NULL){
    printf("Command not found.\n");
    return;
  }
  char *del = strchr(value, '|');
  char run[256];
  if (del != NULL){
    char *run_end = del;
    while (run_end > value && *(run_end - 1) == ' ') run_end--;
    snprintf(run, sizeof(run), "%.*s", (int)(run_end - value), value);
  } else {
    snprintf(run, sizeof(run), "%s", value);
  }
  printf("=== running: %s ===\n", command);
  FILE *output = popen(run, "r");
  if (output == NULL) return;
  char line[256];
  while(fgets(line, 256, output)) {
    printf("%s", line);
  }
  pclose(output);
}
