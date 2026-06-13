#include "utils/hashmap.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h> 
#include <stdlib.h>

void free_str_array(char **arr, int count){
  for (int i = 0; i < count; i++){
    free(arr[i]);
  }
  free(arr);
}

char **get_units(HashMap *units, const char *key, int *count){
  char **units_arr = NULL;
  char *current_unit_ptr = NULL;
  *count = 0;
  char *values = get(units, key);
  if (values == NULL) return NULL;
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
  if (values == NULL) return NULL;
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

void cmd_init(const char *group, const char *units){
  FILE *fptr = fopen("/etc/pmgr/config.ini", "a");
  if (fptr == NULL){
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }
  size_t len = strlen(group) + strlen(units) + 16;
  char *new_group = malloc(len);
  snprintf(new_group, len, "\n[%s]\nunits = %s\n", group, units);
  fwrite(new_group, 1, strlen(new_group), fptr);
  free(new_group);
  fclose(fptr);
  printf("Group %s added.\n", group);
}

void cmd_status(HashMap *units, const char *key){
  int count = 0;
  char **units_arr = get_units(units, key, &count);
  if (units_arr == NULL){
    printf("Group not found.\n");
    return;
  }
  for (int i = 0; i < count; i++){
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "systemctl status %s", units_arr[i]);
    printf("=== %s ===\n", units_arr[i]);
    FILE *output = popen(cmd, "r"); 
    if (output == NULL) continue;
    char line[256];
    while(fgets(line, 256, output)) {
      printf("%s", line);
    }
    pclose(output);
  }
  free_str_array(units_arr, count);
}
void cmd_logs(HashMap *units, const char *key, int max_logs){
  int count = 0;
  char **units_arr = get_units(units, key, &count);
  if (units_arr == NULL){
    printf("Group not found.\n");
    return;
  }
  for (int i = 0; i < count; i++){
    char cmd[256];
    if (!max_logs) snprintf(cmd, sizeof(cmd), "journalctl -u %s", units_arr[i]);
    else snprintf(cmd, sizeof(cmd), "journalctl -u %s -n %d", units_arr[i], max_logs);
    printf("=== %s ===\n", units_arr[i]);
    FILE *output = popen(cmd, "r"); 
    if (output == NULL) continue;
    char line[256];
    while(fgets(line, 256, output)) {
      printf("%s", line);
    }
    pclose(output);
  }
  free_str_array(units_arr, count);
}

void cmd_restart(HashMap *units, const char *key){
  int count = 0;
  char **units_arr = get_units(units, key, &count);
  if (units_arr == NULL){
    printf("Group not found.\n");
    return;
  }
  for (int i = 0; i < count; i++){
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "systemctl restart %s", units_arr[i]);
    printf("=== %s ===\n", units_arr[i]);
    FILE *output = popen(cmd, "r"); 
    printf("Restarted %s\n", units_arr[i]);
    pclose(output);
  }
  free_str_array(units_arr, count);
}

void cmd_custom(HashMap *commands, const char *key){
  int count = 0;
  char **commands_arr = get_custom_commands(commands, key, &count);
  if (commands_arr == NULL){
    printf("No custom commands found for group %s.\n", key);
    return;
  }
  printf("Custom commands for %s:\n", key);
  for (int i = 0; i < count; i++){
    printf("  %s\n", commands_arr[i]);
    char *value = get_command_value(commands, key, commands_arr[i]);
    if (value == NULL) continue;
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
  free_str_array(commands_arr, count);
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

void cmd_create_custom(const char *group, const char *command, const char *script, const char *desc){
  FILE *fptr;
  fptr = fopen("/etc/pmgr/config.ini", "r+");

  if (fptr == NULL) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  fseek(fptr, 0, SEEK_END);
  long file_size = ftell(fptr);
  fseek(fptr, 0, SEEK_SET);
  char *buf_in = malloc(file_size + 1);
  fread(buf_in, 1, file_size, fptr);
  buf_in[file_size] = '\0';

  char section_header[256];
  snprintf(section_header, sizeof(section_header), "[%s:commands]", group);
  char *found = strstr(buf_in, section_header);
  char *buf_out = malloc(file_size + 512 + 1);

  if (found != NULL){
    if (strchr(found+1, '[') != NULL){
      char *next_section = strchr(found+1, '[');

      size_t command_len = strlen(command);
      size_t section_len = (size_t)(next_section - found);

      for (size_t i = 0; i + command_len <= section_len; i++){
        if (strncmp(found+i, command, command_len) == 0){
          size_t peek_idx = i + command_len;
          while (peek_idx < section_len && (found[peek_idx] == ' ' || found[peek_idx] == '\t')) {
            peek_idx++;
          }
          if (peek_idx < section_len && found[peek_idx] == '=') {
            printf("Command already exists.\n");
            free(buf_out);
            free(buf_in);
            fclose(fptr);
            return;
          }
        }
      }

      size_t pos = 0;
      memcpy(buf_out + pos, buf_in, next_section - buf_in);
      pos += next_section - buf_in;
      buf_out[pos] = '\n'; 
      pos += 1;

      char new_line[256];
      if (desc != NULL){
        snprintf(new_line, sizeof(new_line), "%s = %s | %s\n", command, script, desc);
      } else {
        snprintf(new_line, sizeof(new_line), "%s = %s\n", command, script);
      }
      memcpy(buf_out + pos, new_line, strlen(new_line));
      pos += strlen(new_line);
      memcpy(buf_out + pos, next_section, strlen(buf_in) - (next_section - buf_in));
      buf_out[pos + strlen(buf_in) - (next_section - buf_in)] = '\0';
    } else {
      size_t command_len = strlen(command);
      size_t section_len = strlen(found); 

      for (size_t i = 0; i + command_len <= section_len; i++){
        if (strncmp(found+i, command, command_len) == 0){ size_t peek_idx = i + command_len; while (peek_idx < section_len && (found[peek_idx] == ' ' || found[peek_idx] == '\t')) {
            peek_idx++;
          }
          if (peek_idx < section_len && found[peek_idx] == '=') {
            printf("Command already exists.\n");
            free(buf_out);
            free(buf_in);
            fclose(fptr);
            return;
          }
        }
      }

      memcpy(buf_out, buf_in, file_size);
      char new_line[256];
      if (desc != NULL){
        snprintf(new_line, sizeof(new_line), "%s = %s | %s\n", command, script, desc);
      } else {
        snprintf(new_line, sizeof(new_line), "%s = %s\n", command, script);
      }
      memcpy(buf_out + file_size, new_line, strlen(new_line));
      buf_out[file_size + strlen(new_line)] = '\0';
    }
  } else {
    memcpy(buf_out, buf_in, file_size);
    size_t pos = 0;

    char new_line[256];

    if (desc != NULL){
      snprintf(new_line, sizeof(new_line), "%s = %s | %s\n", command, script, desc);
    } else {
      snprintf(new_line, sizeof(new_line), "%s = %s\n", command, script);
    }
    pos += file_size;
    buf_out[pos] = '\n'; 
    pos += 1; 
    memcpy(buf_out + pos, section_header, strlen(section_header));
    buf_out[pos + strlen(section_header)] = '\n';
    pos += strlen(section_header) + 1;
    memcpy(buf_out + pos, new_line, strlen(new_line));
    pos += strlen(new_line);
    buf_out[pos] = '\0';
  }
  fseek(fptr, 0, SEEK_SET);
  fwrite(buf_out, 1, strlen(buf_out), fptr);
  fflush(fptr);
  ftruncate(fileno(fptr), strlen(buf_out));
  fclose(fptr);

  printf("Custom command '%s' added to group '%s'.\n", command, group);

  free(buf_in);
  free(buf_out);
}

void cmd_delete_custom(const char *group, const char *command){
  FILE *fptr = fopen("/etc/pmgr/config.ini", "r+");
  if (fptr == NULL){
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }
  fseek(fptr, 0, SEEK_END);
  long file_size = ftell(fptr);
  fseek(fptr, 0, SEEK_SET);
  char *buf_in = malloc(file_size + 1);
  fread(buf_in, 1, file_size, fptr);
  buf_in[file_size] = '\0';

  char section_header[256];
  snprintf(section_header, sizeof(section_header), "[%s:commands]", group);
  char *found = strstr(buf_in, section_header);
  if (found == NULL){
    printf("Group %s has no custom commands.\n", group);
    free(buf_in);
    fclose(fptr);
    return;
  }

  char *buf_out = malloc(file_size + 1);
  char search[256];
  snprintf(search, sizeof(search), "%s =", command);
  char *cmd_line = strstr(found, search);
  if (cmd_line == NULL){
    printf("Command %s not found.\n", command);
    free(buf_in);
    free(buf_out);
    fclose(fptr);
    return;
  }

  char *line_end = strchr(cmd_line, '\n');
  if (line_end != NULL) line_end++;
  else line_end = cmd_line + strlen(cmd_line);

  size_t pos = 0;
  memcpy(buf_out, buf_in, cmd_line - buf_in);
  pos += cmd_line - buf_in;
  memcpy(buf_out + pos, line_end, strlen(buf_in) - (line_end - buf_in));
  pos += strlen(buf_in) - (line_end - buf_in);
  buf_out[pos] = '\0';

  fseek(fptr, 0, SEEK_SET);
  fwrite(buf_out, 1, strlen(buf_out), fptr);
  fflush(fptr);
  ftruncate(fileno(fptr), strlen(buf_out));
  fclose(fptr);

  printf("Command %s deleted from group %s.\n", command, group);

  free(buf_in);
  free(buf_out);
}

static void remove_section(const char *filename, const char *section_header){
  FILE *fptr = fopen(filename, "r+");
  if (fptr == NULL) return;

  fseek(fptr, 0, SEEK_END);
  long file_size = ftell(fptr);
  fseek(fptr, 0, SEEK_SET);
  char *buf_in = malloc(file_size + 1);
  fread(buf_in, 1, file_size, fptr);
  buf_in[file_size] = '\0';

  char *found = strstr(buf_in, section_header);
  if (found == NULL){
    free(buf_in);
    fclose(fptr);
    return;
  }

  char *buf_out = malloc(file_size + 1);
  char *end = strchr(found + 1, '[');
  if (end == NULL) end = buf_in + file_size;

  size_t pos = 0;
  memcpy(buf_out, buf_in, found - buf_in);
  pos += found - buf_in;
  memcpy(buf_out + pos, end, strlen(buf_in) - (end - buf_in));
  pos += strlen(buf_in) - (end - buf_in);
  buf_out[pos] = '\0';

  fseek(fptr, 0, SEEK_SET);
  fwrite(buf_out, 1, strlen(buf_out), fptr);
  fflush(fptr);
  ftruncate(fileno(fptr), strlen(buf_out));
  fclose(fptr);

  free(buf_in);
  free(buf_out);
}

void cmd_delete_group(const char *group){
  char section_header[256];
  char commands_header[256];
  snprintf(section_header, sizeof(section_header), "[%s]", group);
  snprintf(commands_header, sizeof(commands_header), "[%s:commands]", group);

  FILE *fptr = fopen("/etc/pmgr/config.ini", "r");
  if (fptr == NULL){
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }
  fseek(fptr, 0, SEEK_END);
  long file_size = ftell(fptr);
  fseek(fptr, 0, SEEK_SET);
  char *buf = malloc(file_size + 1);
  fread(buf, 1, file_size, fptr);
  buf[file_size] = '\0';
  fclose(fptr);

  if (strstr(buf, section_header) == NULL){
    printf("Group %s not found.\n", group);
    free(buf);
    return;
  }
  free(buf);

  remove_section("/etc/pmgr/config.ini", section_header);
  remove_section("/etc/pmgr/config.ini", commands_header);

  printf("Group %s deleted.\n", group);
}
