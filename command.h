#include "utils/hashmap.h"

void free_str_array(char **arr, int count);
char **get_units(HashMap *units, const char *key, int *count);
void cmd_init(const char *group, const char *units);
void cmd_reset();
void cmd_status(HashMap *units, const char *key);
void cmd_logs(HashMap *units, const char *key, int max_logs);
void cmd_restart(HashMap *units, const char *key);
void cmd_custom(HashMap *commands, const char *key);
void cmd_run(HashMap *commands, const char *key, const char* command);
void cmd_create_custom(const char *group, const char *command, const char *script, const char *desc);
void cmd_delete_custom(const char *group, const char *command);
void cmd_delete_group(const char *group);
void cmd_view_config();
