#include "cJSON.h"
#include <stdint.h>
size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
int write_file_from_link(char *fileName,char *site);
void parse_file_lignes(const char *filename, char *postal);
void parse_file_metro(char *metro);
char **parse_file_plan(char *metro);
void parse_file_horaires(char *metro, char **stations);
