#include "FirstPass.h"

void freeNodeSrcValue(LineInfo node){

    free(node->srcValue);

}
void freeNodeDesValue(LineInfo node){

    free(node->desValue);
}


char *addOneToBinary(const char *binary)
{
    int length;
    int carry;
    char *result;
    int i;
    char * f_res ;

    length = strlen(binary);
    carry = 1;

    result = (char *)calloc(length + 1, sizeof(char));
    if (result == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    for (i = length - 1; i >= 0; i--)
    {
        if (binary[i] == '0' && carry == 1)
        {
            result[i ] = '1';
            carry = 0;
        }
        else if (binary[i] == '1' && carry == 1)
        {
            result[i ] = '0';
        }
        else
        {
            result[i ] = binary[i];
        }
    }
    if (carry == 1)
    {
        f_res = calloc(length +2, sizeof(char));
        result[0] = '1';
        result[length + 1] = '\0';
        strncpy(f_res,result,strlen(result));
    }
    else
    {
        f_res = calloc(length +1, sizeof(char));
        strncpy(f_res,result ,strlen(result));
    }
        free(result);
    return f_res;
}

void flip_binary_string(char *binary_string)
{
    int i;
    for (i = 0; binary_string[i] != '\0'; i++)
    {
        if (binary_string[i] == '0')
        {
            binary_string[i] = '1';
        }
        else if (binary_string[i] == '1')
        {
            binary_string[i] = '0';
        }
    }
    binary_string = addOneToBinary(binary_string);
}

char *extractBetweenApostrophes(const char *input)
{
    char *output = NULL;
    const char *start;
    const char *end;
    size_t length;
    char *word;
    start = input;
    while ((start = strchr(start, '\"')) != NULL)
    {
        end = strchr(start + 1, '\"');
        if (end == NULL)
        {
            break;
        }

        length = end - start - 1;
        if (length > 0)
        {
            word = (char *)calloc((length + 1), sizeof(char));
            if (word != NULL)
            {
                strncpy(word, start + 1, length);
                word[length] = '\0';

                output = (char *)calloc(strlen(word) + 1, sizeof(char));
                strcpy(output, word);

                free(word);
            }
        }

        start = end + 1;
    }

    return output;
}


int *parseNumbers(const char *input, int *size)
{
    int count;
    int i;
    int *array;
    char *token;
    count = 1;
    for (i = 0; input[i]; i++)
    {
        if (input[i] == ',')
        {
            count++;
        }
    }

    array = (int *)calloc(count, sizeof(int));

    if (array == NULL)
    {
        *size = 0;
        return NULL;
    }

    token = strtok((char *)input, ",");
    for (i = 0; i < count; i++)
    {
        array[i] = atof(token);
        token = strtok(NULL, ",");
    }
    free(token);
    *size = count;
    return array;
}


void buildTwoOperandCodeLine(char *res,  LineInfo lineInfo)
{
    char *actionCode;
    char *destinationOperandBinary;
    char *sourceOperandBinary;
    char *ARE;
    ARE="00";
    actionCode = int_to_binary(lineInfo->actionCode);
    complete_string_size(actionCode, 4);
    sourceOperandBinary = int_to_binary(lineInfo->sourceAddress);
    complete_string_size(sourceOperandBinary, 3);
    destinationOperandBinary = int_to_binary(lineInfo->destinationAddress);
    complete_string_size(destinationOperandBinary, 3);
    strcpy(res, sourceOperandBinary);
    strcat(res, actionCode);
    strcat(res, destinationOperandBinary);
    strcat(res, ARE);
    free(actionCode);
    free(destinationOperandBinary);
    free(sourceOperandBinary);
}


void buildOneOperandCodeLine(char *res,  LineInfo lineInfo)
{
    char *actionCode;
    char *destinationOperandBinary;
    char *sourceOperandBinary;
    char *ARE;
    sourceOperandBinary="000";
    ARE="00";
    actionCode = int_to_binary(lineInfo->actionCode);
    complete_string_size(actionCode, 4);
    destinationOperandBinary = int_to_binary(lineInfo->destinationAddress);
    complete_string_size(destinationOperandBinary, 3);
    strcpy(res, sourceOperandBinary);
    strcat(res, actionCode);
    strcat(res, destinationOperandBinary);
    strcat(res, ARE);
    free(destinationOperandBinary);
    free(actionCode);

}


void buildNoOperandCodeLine(char *res,  LineInfo lineInfo)
{
    char *actionCode;
    char *destinationOperandBinary;
    char *sourceOperandBinary;
    char *ARE;
    sourceOperandBinary="000";
    ARE="00";
    destinationOperandBinary="000";

    actionCode = int_to_binary(lineInfo->actionCode);
    complete_string_size(actionCode, 4);
    strcpy(res, sourceOperandBinary);
    strcat(res, actionCode);
    strcat(res, destinationOperandBinary);
    strcat(res, ARE);
    free(actionCode);
}


void build2RegisterString(char *res, char *sourceOperand, char *destinationOperand)
{
    char *start;
    char *destinationOperandBinary;
    char *sourceOperandBinary;
    char *ARE;
    char *middle;
    start="00";
    middle="00";
    ARE="00";
    sourceOperandBinary = int_to_binary(strtol(sourceOperand, NULL, 10));
    destinationOperandBinary = int_to_binary(strtol(destinationOperand, NULL, 10));
    complete_string_size(sourceOperandBinary, 3);
    complete_string_size(destinationOperandBinary, 3);
    strcpy(res, start);
    strcat(res, sourceOperandBinary);
    strcat(res, middle);
    strcat(res, destinationOperandBinary);
    strcat(res, ARE);

    free(destinationOperandBinary);
    free(sourceOperandBinary);
}


void buildSourceRegisterString(char *res, char *sourceOperand)
{
    char *start;
    char *middle;
    char *sourceOperandBinary;
    char *ARE;
    start="00";
    middle="00";
    ARE="00";
    sourceOperandBinary = int_to_binary(strtol(sourceOperand, NULL, 10));
    complete_string_size(sourceOperandBinary, 3);
    strcpy(res, start);
    strcat(res, sourceOperandBinary);
    strcat(res, middle);
    strcat(res, "000");
    strcat(res, ARE);
    free(sourceOperandBinary);
    
}


void buildDestinationRegisterString(char *res, char *destinationOperand)
{
    char *start;
    char *middle;
    char *destinationOperandBinary;
    char *ARE;
    start ="00";
    middle="00";
    ARE="00";
    destinationOperandBinary = int_to_binary(strtol(destinationOperand, NULL, 10));
    complete_string_size(destinationOperandBinary, 3);
    strcpy(res, start);
    strcat(res, "000");
    strcat(res, middle);
    strcat(res, destinationOperandBinary);
    strcat(res, ARE);
    free(destinationOperandBinary);

}


void buildNumberData(char **res, char *dataAsString, int *line)
{
    int size;
    int *number;
    int i;
    char *binaryString;

    number = parseNumbers(dataAsString, &size);
    for (i = 0; i < size; i++)
    {
        res[i] = (char *)calloc(13, sizeof(char));

        if (number[i] < 0)
        {
            binaryString = int_to_binary(number[i] * -1);
            complete_string_size(binaryString, 12);
            flip_binary_string(binaryString);
            binaryString = addOneToBinary(binaryString);
        }
        else
        {
            binaryString = int_to_binary(number[i]);
            complete_string_size(binaryString, 12);
        }
        strcpy(res[i], binaryString);
    }
    line[0] = line[0] + size;
    free(number);
    free(binaryString);
}


long getAsciiCodeAsLong(char inputChar)
{
    return (long)inputChar;
}


void buildStringData(char **res, char *string)
{
    char *bunaryAsciiCodeVaulue;
    int size;
    int i;
    size = strlen(string);

    for (i = 0; i <= size; i++)
    {
        res[i] = (char *)calloc(13, sizeof(char));
        bunaryAsciiCodeVaulue = int_to_binary(getAsciiCodeAsLong(string[i]));
        complete_string_size(bunaryAsciiCodeVaulue, 12);
        strcpy(res[i], bunaryAsciiCodeVaulue);
    }
    free(bunaryAsciiCodeVaulue);
}


void buildNumberString(char *res, char *number)
{
    int num;
    char *numberBinary;
    char *ARE;
    num = strtol(number, NULL, 10);
    ARE="00";
    if (num < 0)
    {
        numberBinary = int_to_binary(num * -1);
        complete_string_size(numberBinary, 10);
        flip_binary_string(numberBinary);
        numberBinary = addOneToBinary(numberBinary);
    }
    else
    {
        numberBinary = int_to_binary(num);
        complete_string_size(numberBinary, 10);
    }
    strncpy(res, numberBinary, 10);
    strcat(res, ARE);
    free(numberBinary);
}


void fillLineFirstPass( LineInfo lineInfo, int *line)
{
    int currentLine;
    lineInfo->startLine = *line;
    if (lineInfo->actionFamily == 0)
    {
        lineInfo->res[0] = (char *)calloc(13, sizeof(char));
        buildTwoOperandCodeLine(lineInfo->res[0], lineInfo);
        line[0]++;
        (lineInfo->numberOfLines)++;
        if ((int)strcmp(int_to_binary(lineInfo->destinationAddress), "101") == 0)
        {
            if ((int)strcmp(int_to_binary(lineInfo->sourceAddress), "101") == 0)
            {
                lineInfo->res[1] = (char *)calloc(13, sizeof(char));
                build2RegisterString(lineInfo->res[1], lineInfo->srcValue, lineInfo->desValue);
                line[0]++;
                (lineInfo->numberOfLines)++;
                freeNodeDesValue(lineInfo);
                freeNodeSrcValue(lineInfo);
                return;
            }
            if ((int)strcmp(int_to_binary(lineInfo->sourceAddress), "11") == 0)
            {
                lineInfo->res[2] = (char *)calloc(13, sizeof(char));
                buildDestinationRegisterString(lineInfo->res[2], lineInfo->desValue);
                line[0]++;
                line[0]++;
                (lineInfo->numberOfLines)++;
                (lineInfo->numberOfLines)++;
                freeNodeDesValue(lineInfo);
                return;
            }
            if ((int)strcmp(int_to_binary(lineInfo->sourceAddress), "1") == 0)
            {

                lineInfo->res[1] = (char *)calloc(13, sizeof(char));
                lineInfo->res[2] = (char *)calloc(13, sizeof(char));
                buildNumberString(lineInfo->res[1], lineInfo->srcValue);
                buildDestinationRegisterString(lineInfo->res[2], lineInfo->desValue);
                line[0]++;
                line[0]++;
                (lineInfo->numberOfLines)++;
                (lineInfo->numberOfLines)++;
                freeNodeDesValue(lineInfo);

                freeNodeSrcValue(lineInfo);
                return;
            }
        }
        else if ((int)strcmp(int_to_binary(lineInfo->destinationAddress), "11") == 0)
        {
            line[0]++;
            (lineInfo->numberOfLines)++;
            if ((int)strcmp(int_to_binary(lineInfo->sourceAddress), "101") == 0)
            {
                lineInfo->res[1] = (char *)calloc(13, sizeof(char));
                buildSourceRegisterString(lineInfo->res[1], lineInfo->srcValue);
                line[0]++;
                (lineInfo->numberOfLines)++;
                freeNodeSrcValue(lineInfo);
                return;
            }
            if ((int)strcmp(int_to_binary(lineInfo->sourceAddress), "1") == 0)
            {
                lineInfo->res[1] = (char *)calloc(13, sizeof(char));
                buildNumberString(lineInfo->res[1], lineInfo->srcValue);
                line[0]++;
                (lineInfo->numberOfLines)++;
                freeNodeSrcValue(lineInfo);
                return;
            }
            if ((int)strcmp(int_to_binary(lineInfo->sourceAddress), "11") == 0)
            {
                line[0]++;
                (lineInfo->numberOfLines)++;
                return;
            }
        }
        else if ((int)strcmp(int_to_binary(lineInfo->destinationAddress), "1") == 0)
        {
            lineInfo->res[2] = (char *)calloc(13, sizeof(char));
            buildNumberString(lineInfo->res[2], lineInfo->desValue);
            line[0]++;
            (lineInfo->numberOfLines)++;
            if ((int)strcmp(int_to_binary(lineInfo->sourceAddress), "101") == 0)
            {
                lineInfo->res[1] = (char *)calloc(13, sizeof(char));
                buildSourceRegisterString(lineInfo->res[1], lineInfo->srcValue);
                line[0]++;
                (lineInfo->numberOfLines)++;
                freeNodeDesValue(lineInfo);
                freeNodeSrcValue(lineInfo);
                return;
            }
            if ((int)strcmp(int_to_binary(lineInfo->sourceAddress), "11") == 0)
            {
                line[0]++;
                (lineInfo->numberOfLines)++;
                freeNodeDesValue(lineInfo);
                return;
            }
            if ((int)strcmp(int_to_binary(lineInfo->sourceAddress), "1") == 0)
            {
                lineInfo->res[1] = (char *)calloc(13, sizeof(char));
                buildNumberString(lineInfo->res[1], lineInfo->srcValue);
                line[0]++;
                (lineInfo->numberOfLines)++;
                freeNodeDesValue(lineInfo);
                freeNodeSrcValue(lineInfo);
                return;
            }
        }
    }
    if (lineInfo->actionFamily == 1)
    {
        lineInfo->res[0] = (char *)calloc(13, sizeof(char));
        buildOneOperandCodeLine(lineInfo->res[0], lineInfo);
        line[0]++;
        (lineInfo->numberOfLines)++;
        if ((int)strcmp(int_to_binary(lineInfo->destinationAddress), "1") == 0)
        {
            lineInfo->res[1] = (char *)calloc(13, sizeof(char));
            buildNumberString(lineInfo->res[1], lineInfo->desValue);
            line[0]++;
            (lineInfo->numberOfLines)++;
            freeNodeDesValue(lineInfo);
            return;
        }
        if ((int)strcmp(int_to_binary(lineInfo->destinationAddress), "11") == 0)
        {
            line[0]++;
            (lineInfo->numberOfLines)++;
            return;
        }
        if ((int)strcmp(int_to_binary(lineInfo->destinationAddress), "101") == 0)
        {
            lineInfo->res[1] = (char *)calloc(13, sizeof(char));
            buildDestinationRegisterString(lineInfo->res[1], lineInfo->desValue);
            line[0]++;
            (lineInfo->numberOfLines)++;
            freeNodeDesValue(lineInfo);
            return;
        }
    }
    if (lineInfo->actionFamily == 2)
    {
        lineInfo->res[0] = (char *)calloc(13, sizeof(char));
        buildNoOperandCodeLine(lineInfo->res[0], lineInfo);
        line[0]++;
        (lineInfo->numberOfLines)++;
    }

    if (lineInfo->actionFamily == 3)
    {
        char* string;
        string = extractBetweenApostrophes(lineInfo->desValue);
        if (string == NULL)
        {
            lineInfo->res[0] = "Missing Quote Sign";
            lineInfo->numberOfLines = 0;
            free(string);
            freeNodeDesValue(lineInfo);
            return;
        }

        buildStringData(lineInfo->res, string);
        line[0] = line[0] + strlen(string) + 1;
        lineInfo->numberOfLines = lineInfo->numberOfLines + strlen(string) + 1;
        freeNodeDesValue(lineInfo);
        free(string);
    }
    if (lineInfo->actionFamily == 4)
    {

        currentLine = *line;
        buildNumberData(lineInfo->res, lineInfo->desValue, line);
        lineInfo->numberOfLines = *line - currentLine;
        freeNodeDesValue(lineInfo);
    }
}
