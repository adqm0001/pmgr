#include <stdio.h>
#include <stdlib.h>
#include "utils/hashmap.h"
#include "config.h"
#include <string.h>
#include "command.h"

int main(int argc, char *argv[]){
  if (argc < 3) {
    printf("Not enough arguments.\n");
    return 1;
  } else if (argc > 5){
    printf("Too many arguments.\n");
    return 1;
  } else if (argc == 4){
    printf("Incorrect usage\n");
    return 1;
  }

  const char *program_name = argv[0];
  const char *command = argv[1];
  const char *optional_max_logs;
  int logs_limit = 0;
  const char *group;

  if (argc == 5) {
    if (strcmp(command, "logs")) {
      printf("Additional argument only applies to logs command.\n");
      return 1;
    }
    group = argv[2];
    if (strcmp(argv[3], "-n")) {
      printf("Error: unknown flag %s\n", argv[3]);
      return 1;
    }
    if (atoi(argv[4]) <= 0) {
      printf("Error: number of logs must be greater than 0.\n");
      return 1;
    }
    optional_max_logs = argv[4];
    logs_limit = 1;
  } else if (argc == 3) {
    group = argv[2];
  }

  printf("command: %s\n", command);
  printf("group: %s\n", group);

  HashMap *hashmap = hashmap_create(10);

  load_config(hashmap);
  printf("-----------------------\n");

  const char *status = "status";
  const char *logs = "logs";
  const char *restart = "restart";
  if (!(strcmp(command, status))){
    cmd_status(hashmap, group);
  } else if (!(strcmp(command, logs))){
    int max_logs = 0;
    if (logs_limit){
      max_logs = atoi(optional_max_logs);
    }
    cmd_logs(hashmap, group, max_logs);
  } else if (!(strcmp(command, restart))){
    cmd_restart(hashmap, group);
  }

  free_hashmap(hashmap);

  return 0;
}

