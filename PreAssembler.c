#include "PreAssembler.h"



 KeyValuePair create_key_value_pair(const char *key, const char *value) {
     KeyValuePair pair = ( KeyValuePair )malloc(sizeof(struct KeyValuePair));
    if (pair == NULL) {
        perror("Memory allocation error");
        return NULL;
    }


    pair->key = (char *)malloc(strlen(key) + 1);
    if (pair->key != NULL) {
    strcpy(pair->key, key);
    }

    pair->value = (char *)malloc(strlen(value) + 1);
    if (pair->value != NULL) {
    strcpy(pair->value, value);
    }
    pair->next = NULL;

    return pair;
}

void free_key_value_pair( KeyValuePair pair) {
    if (pair == NULL) {
        return;
    }

    free(pair->key);
    free(pair->value);
    free(pair);
}

void add_key_value_pair( KeyValuePair *list, const char *key, const char *value) {
     KeyValuePair new_pair = create_key_value_pair(key, value);
    if (new_pair == NULL) {
        return;
    }

    if (*list == NULL) {
        *list = new_pair;
    } else {
         KeyValuePair current = *list;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_pair;
    }
}
void free_key_value_list( KeyValuePair list) {
    while (list != NULL) {
         KeyValuePair temp = list;
        list = list->next;
        free_key_value_pair(temp);
    }
}

char *replace_keys( KeyValuePair list, char *line) {
    KeyValuePair current;
    char *result;
    result = (char *)malloc(strlen(line) + 1);
    if (result != NULL) {
    strcpy(result, line);
    }   

    current = list;
    while (current != NULL) {
        char *key;
        char *pos;
        char *value;
        value = current->value;
        key = current->key;
        pos = strstr(result, key);
        while (pos != NULL) {
            size_t key_len = strlen(key);
            size_t value_len = strlen(value);
            size_t pos_index = pos - result;

           
            result = (char *)realloc(result, strlen(result) + value_len - key_len + 1);

            
            memmove(result + pos_index + value_len, result + pos_index + key_len, strlen(result) - pos_index - key_len + 1);

            
            strncpy(result + pos_index, value, value_len);

           
            pos = strstr(result + pos_index + value_len, key);
        }

        current = current->next;
    }

    return result;
}

char*  process_file_pre_assembler(char *input_file_path) {
    char *output_file_path;
    char* filename;
    const char *start_tag;
    const char *end_tag;
    KeyValuePair key_value_list;
    FILE *input_file;
    FILE *output_file;
    char buffer[1024];
    char current_key[256];
    char current_value[1024];
    int in_block;
    output_file_path = (char*)calloc(strlen(input_file_path)+strlen(".am")+1,sizeof(char));
    strcpy(output_file_path, input_file_path);
    strcat(output_file_path, ".am");
    filename = (char *)calloc((strlen(input_file_path)+strlen(".as") + 1), sizeof(char));
    strcpy(filename, input_file_path);
    strcat(filename, ".as");
    start_tag = "mcro";
    end_tag = "endmcro";
    key_value_list = NULL;
    in_block = 0;
    strcpy(current_key,"");
    strcpy(current_value,"");

    input_file = fopen(filename, "r");
    if (input_file == NULL) {
        perror("Error opening input file");
        return NULL;
    }

    output_file = fopen(output_file_path, "w");
    if (output_file == NULL) {
        perror("Error opening output file");
        fclose(input_file);
        return NULL;
    }

    while (fgets(buffer, sizeof(buffer), input_file) != NULL) {
        if (!in_block && strncmp(buffer, start_tag, strlen(start_tag)) == 0) {
            in_block = 1;
            sscanf(buffer, "%*s %s", current_key); 
            current_value[0] = '\0'; 
        } else if (in_block && strncmp(buffer, end_tag, strlen(end_tag)) == 0) {
            in_block = 0;
            add_key_value_pair(&key_value_list, current_key, current_value);
        } else if (in_block) {
            if (strlen(current_value) + strlen(buffer) < sizeof(current_value)) {
                strcat(current_value, buffer);
            } else {
                perror("Value buffer overflow");
                free_key_value_list(key_value_list);
                fclose(input_file);
                fclose(output_file);
                return NULL;
            }
        }
        else {
       
        char *replaced_line = replace_keys(key_value_list, buffer);
       
        if (strstr(replaced_line, start_tag) == NULL && strstr(replaced_line, end_tag) == NULL) {
            fputs(replaced_line, output_file);
        }
        free(replaced_line);
        }
        
    }


    fclose(input_file);
    fclose(output_file);

    
    free_key_value_list(key_value_list);
    return input_file_path;
}
