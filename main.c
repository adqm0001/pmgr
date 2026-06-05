#include <stdio.h>
#include "utils/hashmap.h"
#include "config.h"
#include <string.h>
#include "command.h"

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
  printf("-----------------------\n");

  const char *status = "status";
  const char *logs = "logs";
  const char *restart = "restart";
  if (!(strcmp(command, status))){
    cmd_status(hashmap, group);
  } else if (!(strcmp(command, logs))){
    cmd_logs(hashmap, group);
  } else if (!(strcmp(command, restart))){
    cmd_restart(hashmap, group);
  }

  free_hashmap(hashmap);

  return 0;
}

