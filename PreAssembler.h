#ifndef _PRE_ASSEMBLER_H
#define _PRE_ASSEMBLER_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "HeaderLibraries.h"
#include "Defenitions.h"

typedef struct KeyValuePair *KeyValuePair;
typedef struct KeyValuePair {
    char *key;
    char *value;
    struct KeyValuePair *next;
}keyValuePair;
/* free the value key  */
void free_key_value_pair(struct KeyValuePair *pair);
/* insert keys to the list */
void add_key_value_pair(struct KeyValuePair **list, const char *key, const char *value);
/* free the key list */
void free_key_value_list(struct KeyValuePair *list);
/* replace the macro name with the lines */
char *replace_keys(struct KeyValuePair *list, char *line);
/* go through the file */
char * process_file_pre_assembler( char *input_file_path);
#endif