#include "SecondPass.h"




char *int_to_binary(long num)
{
    int size;
    char *binary_str;
    int found_non_zero;
    int idx;
    int i;
    int bit;

    size = sizeof(int) * 8;

    binary_str = (char *)calloc(size + 1, sizeof(char));

    if (binary_str == NULL)
    {
        printf("Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    idx = 0;

    found_non_zero = 0;

    for (i = size - 1; i >= 0; i--)
    {
        bit = (num >> i) & 1;

        if (bit || found_non_zero)
        {
            binary_str[idx++] = bit + '0';
            found_non_zero = 1;
        }
    }

    if (!found_non_zero)
    {
        binary_str[0] = '0';
        binary_str[1] = '\0';
    }
    else
    {
        binary_str[idx] = '\0';
    }
    return binary_str;
}


void complete_string_size(char *input_string, int size)
{
    int input_length;
    int padding_size;
    char padding[MAX_LINE_LENGTH];
    input_length = strlen(input_string);
    if (input_length >= size)
    {
        return;
    }

    padding_size = size - input_length;
    memset(padding, '0', padding_size);
    padding[padding_size] = '\0';

    memmove(input_string + padding_size, input_string, input_length + 1);
    strncpy(input_string, padding, padding_size);
}


int get_label_line_number_decimal( LabelNode head, const char *line)
{
    

    while (head != NULL)
    {
        
        if ((int)strcmp(line, head->label) == 0)
        {
            return head->lineNumber;
        }
        head = head->next;
    }
    return -1;
}


int get_labelType_line_number_decimal( LabelType head, const char *line, int lineNumber)
{
    
    while (head != NULL)
    {
        
        if ((int)strcmp(line, head->label) == 0)
        {
            if (head->counter)
            {
            }
            else
            {
                head->counter = 0;
            }
            if (head->type == 0)
            {
                head->lines[0] = lineNumber;
                return head->type;
            }
            if (head->type == 1)
            {
                head->lines[head->counter] = lineNumber;
                head->counter++;
                return head->type;
            }
        }
        head = head->next;
    }
    return -1;
}


void buildLabelString(char *res, char *label)
{
    char *ARE;
    ARE = "10";
    complete_string_size(label, 10);
    strcpy(res, label);
    strcat(res, ARE);
}


void secondPass( LineInfo lineInfo,  LabelNode *head,  LabelType *labelType)
{
    int labelLineNumber;
    char *ARE;
    int num;
    while (lineInfo)
    {

        if ((int)strcmp(int_to_binary(lineInfo->sourceAddress), "11") == 0)
        {
            labelLineNumber = get_label_line_number_decimal(*head, lineInfo->srcValue);
            if (labelLineNumber != -1)
            {
                lineInfo->res[1] = (char *)calloc(13, sizeof(char));
                buildLabelString(lineInfo->res[1],
                                 int_to_binary(labelLineNumber));
                get_labelType_line_number_decimal(*labelType, lineInfo->srcValue, labelLineNumber);
            }
            else if (get_labelType_line_number_decimal(*labelType, lineInfo->srcValue, lineInfo->startLine + 1) != -1)
            {
                lineInfo->res[1] = (char *)calloc(13, sizeof(char));
                ARE = "000000000001";
                strcpy(lineInfo->res[1], ARE);
            }
            else
            {
                printf("Error: label is not defined Label: %s \n", lineInfo->srcValue);
                lineInfo->numberOfLines -= 1;
            }
        }
        if ((int)strcmp(int_to_binary(lineInfo->destinationAddress), "11") == 0)
        {
            labelLineNumber = get_label_line_number_decimal(*head, lineInfo->desValue);

            if (lineInfo->actionFamily == 0)
            {
                num = 2;
            }
            else if (lineInfo->actionFamily == 1)
            {
                num = 1;
            }
            if (labelLineNumber != -1)
            {
                lineInfo->res[num] = (char *)calloc(13, sizeof(char));
                buildLabelString(lineInfo->res[num],int_to_binary(labelLineNumber));            
                get_labelType_line_number_decimal(*labelType, lineInfo->desValue, labelLineNumber);
            }
            else if (get_labelType_line_number_decimal(*labelType, lineInfo->desValue, lineInfo->startLine + num) != -1)
            {

                lineInfo->res[num] = (char *)calloc(13, sizeof(char));
                ARE = "000000000001";
                strcpy(lineInfo->res[num], ARE);
            }
            else
            {
                printf("Error: label is not defined Label: %s \n", lineInfo->srcValue);
                lineInfo->numberOfLines -= 1;
            }
        }
        lineInfo = lineInfo->next;
    }
}
