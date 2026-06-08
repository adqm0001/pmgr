#include "utils/hashmap.h"
char **get_units(HashMap *units, const char *key, int *count);
void cmd_status(HashMap *units, const char *key);
void cmd_logs(HashMap *units, const char *key, int max_logs);
void cmd_restart(HashMap *units, const char *key);
void cmd_custom(HashMap *commands, const char *key);
void cmd_run(HashMap *commands, const char *key, const char* command);
//pmgr custom <group> Will list all the custom commands and there description or use case
//pmgr run <group> <command> Will run the command on specified group 
