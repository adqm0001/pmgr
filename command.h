#include "utils/hashmap.h"
char **get_units(HashMap *hashmap, const char *key, int *count);
void cmd_status(HashMap *hashmap, const char *key);
void cmd_logs(HashMap *hashmap, const char *key, int max_logs);
void cmd_restart(HashMap *hashmap, const char *key);


