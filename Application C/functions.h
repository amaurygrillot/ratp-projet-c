#include "cJSON.h"
#include <stdint.h>
int write_file_from_link(char *fileName,char *site);
cJSON *parse_file_lignes(const char *filename, char *postal);
cJSON *parse_file_metro(char *metro);
