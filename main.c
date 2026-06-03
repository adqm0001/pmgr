#include <stdio.h>

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
  return 0;
}

