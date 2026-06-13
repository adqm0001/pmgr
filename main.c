#include <stdio.h>
#include <stdlib.h>
#include "utils/hashmap.h"
#include "config.h"
#include <string.h>
#include "command.h"

void free_hashmaps(HashMap *units, HashMap *commands){
  free_hashmap(units);
  free_hashmap(commands);
}

int main(int argc, char *argv[]){
  if (argc < 2) {
    printf("No command given.\n");
    return 1;
  } else if (argc > 7) {
    printf("Too many arguments.\n");
    return 1;
  } else if (argc == 6) {
    printf("Incorrect usage.\n");
    return 1;
  }

  const char *valid_commands[] = {"status", "logs", "restart", "custom", "run", "help", "init", "create-custom", "delete-custom", "delete-group"}; 
  int num_commands = 10;
  char *command = argv[1];
  int found = 0;
  HashMap *units = hashmap_create(10);
  HashMap *commands = hashmap_create(10);
  load_config(units, commands);

  for (int i = 0; i < num_commands; i++){
    if (strcmp(command, valid_commands[i]) == 0){
      found = 1; 
    }
  }
  if (!found){
    printf("Command not found.\n");
    free_hashmaps(units, commands);
    return 1;
  }
/*
pmgr help
pmgr init <group> <units>
pmgr status <group>
pmgr logs <group>
pmgr logs <group> -n <number>
pmgr restart <group>
pmgr custom <group>
pmgr run <group> <command>
pmgr create-custom <group> <command> "<shell command>"
pmgr create-custom <group> <command> "<shell command>" -d "<description>"
pmgr delete-custom <group> <command>
pmgr delete-group <group>
*/

  // Check if agrc == 2 so either help or wrong command/usage
  if (argc == 2) {
    if (strcmp(command, "help") == 0){
      printf("Usage:\n");
      printf("  pmgr help\n");
      printf("  pmgr init <group> <units>\n");
      printf("  pmgr status <group>\n");
      printf("  pmgr logs <group>\n");
      printf("  pmgr logs <group> -n <number>\n");
      printf("  pmgr restart <group>\n");
      printf("  pmgr custom <group>\n");
      printf("  pmgr run <group> <command>\n");
      printf("  pmgr create-custom <group> <command> \"<shell command>\" -d \"<description>\"\n");
      printf("  pmgr delete-custom <group> <command>\n");
      printf("  pmgr delete-group <group>\n");
    } else {
      printf("Incorrect usage.\n"); 
      free_hashmaps(units, commands);
      return 1;
    }
  }
  
  // We now check if agrc == 3 so either status, logs (no -n flag), restart, custom, delete-group or incorrect usage.
  if (argc == 3) {
    if (strcmp(command, "status") == 0){
      cmd_status(units, argv[2]);
    } else if (strcmp(command, "logs") == 0){
      cmd_logs(units, argv[2], 0);
    } else if (strcmp(command, "restart") == 0){
      cmd_restart(units, argv[2]);
    } else if (strcmp(command, "custom") == 0){
      cmd_custom(commands, argv[2]);
    } else if (strcmp(command, "delete-group") == 0){
      cmd_delete_group(argv[2]);
    } else {
      printf("Incorrect usage.\n"); 
      free_hashmaps(units, commands);
      return 1;
    }
  }

  // We now check if argc == 4 so run, delete-custom, init or incorrect usage 
  if (argc == 4) {
    if (strcmp(command, "run") == 0){
      cmd_run(commands, argv[2], argv[3]);
    } else if (strcmp(command, "delete-custom") == 0){
      cmd_delete_custom(argv[2], argv[3]);
    } else if (strcmp(command, "init") == 0){
      cmd_init(argv[2], argv[3]);
    } else if (strcmp(command, "create-custom") == 0){
      printf("Incorrect usage.\n");
      free_hashmaps(units, commands);
      return 1;
    } else {
      printf("Incorrect usage.\n"); 
      free_hashmaps(units, commands);
      return 1;
    }
  }
  
  // We now check if agrc == 5 so logs (with -n flag), create-custom (no desc) or incorrect usage
  // pmgr create-custom <group> <command> "<shell command>"
  if (argc == 5) {
    if (strcmp(command, "logs") == 0){
      if (strcmp(argv[3], "-n")) {
        printf("Error: unknown flag %s\n", argv[3]);
        free_hashmaps(units, commands);
        return 1;
      } else if (atoi(argv[4]) <= 0) {
        printf("Error: number of logs must be greater than 0.\n"); 
        free_hashmaps(units, commands);
        return 1;
      } else {
        char *group = argv[2];
        int max_logs = atoi(argv[4]);
        cmd_logs(units, group, max_logs);
      }
    } else if (strcmp(command, "create-custom") == 0){
      cmd_create_custom(argv[2], argv[3], argv[4], NULL);
    } else {
      printf("Incorrect usage.\n"); 
      free_hashmaps(units, commands);
      return 1;
    }
  }

  // We now check if argc == 7 so create-custom command or incorrect usage
  // pmgr create-custom <group> <command> "<shell command>" -d "<description>"
  if (argc == 7) {
    if (strcmp(command, "create-custom") == 0) {
      if (strcmp(argv[5], "-d")) {
        printf("Error: unknown flag %s\n", argv[5]);
        free_hashmaps(units, commands);
        return 1;
      } else {
        cmd_create_custom(argv[2], argv[3], argv[4], argv[6]);
      }
    } else {
      printf("Incorrect usage.\n");
      free_hashmaps(units, commands);
      return 1;
    }
  } 

  free_hashmaps(units, commands);

  return 0;
}
