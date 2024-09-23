#include "Assembler.h"


void exportFiles(struct LabelType *head, char *original_file_name)
{
    FILE *externalFile;
    FILE *entryFile;
    char *entryFileName;
    char *externFileName;
    int i;
    struct LabelType *current;
    entryFile = NULL;
    externalFile = NULL;
    entryFileName = (char *)calloc((strlen(original_file_name) + 5), sizeof(char));
    externFileName = (char *)calloc((strlen(original_file_name) + 5), sizeof(char));
    strcpy(entryFileName, original_file_name);
    strcat(entryFileName, ".ent");
    strcpy(externFileName, original_file_name);
    strcat(externFileName, ".ext");

    current = head;

    while (current != NULL)
    {
        if (current->type == 0)
        {
            if (!entryFile)
            {
                entryFile = fopen(entryFileName, "w");
                if (!entryFile)
                {
                    printf("Error opening entries.ent for writing.\n");
                    return;
                }
            }

            fprintf(entryFile, "%s %d\n", current->label, current->lines[0]);
        }
        else if (current->type == 1)
        {

            for ( i = 0; i < current->counter; i++)
            {
                if (i == 0 && !externalFile)
                {
                    externalFile = fopen(externFileName, "w");
                    if (!externalFile)
                    {
                        printf("Error opening externals.ext for writing.\n");
                        return;
                    }
                }
                fprintf(externalFile, "%s %d\n", current->label, current->lines[i]);
            }
        }
        current = current->next;
    }
    free(entryFileName);
    free(externFileName);

    if (entryFile)
    {
        fclose(entryFile);
    }
    if (externalFile)
    {
        fclose(externalFile);
    }
}

void printResForObject(struct LineInfo *lineInfo, int ic, int dc, char *original_file_name)
{
    int base = 1;
    char *output_address;
    FILE *outputFile;
    int i;
    int j;
    char binaryLine[13];
    unsigned int decimalValue;
    const char *base64Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; /* pointer to a litreal string */

    if (lineInfo == NULL)
    {
        return;
    }

    output_address = (char *)calloc(strlen(original_file_name) + strlen(".ob") + 1, sizeof(char));
    strcpy(output_address, original_file_name);
    strcat(output_address, ".ob");

    outputFile = fopen(output_address, "w");
    if (outputFile == NULL)
    {
        printf("Error opening output file.\n");
        return;
    }
    fprintf(outputFile, "%d ", ic);
    fprintf(outputFile, "%d\n", dc);

    while (lineInfo)
    {
        for (i = 0; i < lineInfo->numberOfLines; i++)
        {

            strcpy(binaryLine, lineInfo->res[i]);

            /*  Convert binary to decimal */
            decimalValue = 0;
            base =1;
            for (j = 11; j >=0; j--)
            {
                if(binaryLine[j] == '1') {
                    decimalValue+=base;
                }
                base*=2;
            }

            fprintf(outputFile, "%c%c\n",(base64Chars[(decimalValue >> 6 ) & 0x3f] ),(base64Chars[decimalValue & 0x3f ] ) );

            
        }
        lineInfo = lineInfo->next;
    }
    free(output_address);
    fclose(outputFile);
}

void moveNodesToEnd(struct LineInfo **head)
{
    struct LineInfo *prev;
    struct LineInfo *current;
    int jmpLineNumbers;
    int minusLineNumbers;
    struct LineInfo *temp;
    current = *head;
    prev = NULL;

    if (*head == NULL)
    {
        return;
    }

    while (current != NULL)
    {
        jmpLineNumbers = 0;
        minusLineNumbers = 0;
        if ((current->actionFamily == 3 || current->actionFamily == 4) && current->wasMoved == 0)
        {
            current->wasMoved = 1;
            minusLineNumbers = current->numberOfLines;
            if (prev != NULL)
            {
                prev->next = current->next;

                /*  Find the end of the list and append the current node */
                temp = current->next;
                while (temp->next != NULL)
                {
                    jmpLineNumbers += temp->numberOfLines;
                    temp->startLine = temp->startLine - minusLineNumbers;
                    temp = temp->next;
                }
                jmpLineNumbers += temp->numberOfLines;
                current->startLine = current->startLine + jmpLineNumbers;
                temp->startLine = temp->startLine - minusLineNumbers;
                temp->next = current;
                current->next = NULL;
            }
            else
            {
                /* Moving the first node */
                *head = current->next;
                current->next = NULL;

                temp = *head;
                while (temp->next != NULL)
                {
                    jmpLineNumbers += temp->numberOfLines;
                    temp->startLine = temp->startLine - minusLineNumbers;
                    temp = temp->next;
                }
                current->startLine = current->startLine + jmpLineNumbers;
                temp->next = current;
            }
        }
        else
        {
            prev = current;
        }
        if(prev!=NULL){
	 current = prev->next;
}
  }
}

int is_valid_label(const char *word)
{
    size_t length;
    size_t i;
    length = strlen(word);

    if (length == 0 || length > MAX_LABEL_LENGTH)
        return 0;

    for (i = 0; i < length - 1; i++)
    {
        if (!isalnum(word[i]))
        {
            return 0;
        }
    }

    return word[length - 1] == ':';
}

int insert_label_type(struct LabelType **head, const char *input, struct SourceFileLines *fileLines)
{
    int flag;
    int start;
    char *result;
    const char *secondWordStart;
    const char *secondWordEnd;
    size_t secondWordLength;
    struct LabelType *newNode;
    struct LabelType *current;
    int f;
    int len;
    
    flag = 2;

    start = 0;
    while (input[start] == ',' || input[start] == '\n' || input[start] == '\t' || input[start] == ' ')
    {
        start++;
    }
    input = input + start;

    if (strncmp(input, ".entry", 6) == 0)
    {
        flag = 0;
    }
    if (strncmp(input, ".extern", 7) == 0)
    {
        flag = 1;
    }
    if (flag == 2)
    {
        return flag;
    }
    result = NULL;
    secondWordStart = strchr(input, ' '); 
    if (secondWordStart != NULL)
    {

        while (*secondWordStart == ' ')
        {
            secondWordStart++;
        }

        secondWordEnd = strchr(secondWordStart, ' ');
        if (secondWordEnd != NULL)
        {

            secondWordLength = secondWordEnd - secondWordStart;

            result = (char *)calloc(secondWordLength + 1, sizeof(char));
            if (result != NULL)
            {

                strncpy(result, secondWordStart, secondWordLength);
            }
        }
        else
        {
            f = 0;
            if (secondWordStart[strlen(secondWordStart) - 1] == '\n')
            {
                f = 1;
            }
            result = (char *)calloc(strlen(secondWordStart) + 1, sizeof(char));
            strncpy(result, secondWordStart, strlen(secondWordStart) - f);
            
        }
    }
    if (result != NULL)
    {
        newNode = (struct LabelType *)calloc(1, sizeof(struct LabelType));
        if (newNode == NULL)
        {
            printf("Memory allocation error.\n");
            exit(EXIT_FAILURE);
        }
        CleanString(result,newNode->label);
        newNode->lineContent = (char *)calloc(strlen(input) + 1, sizeof(char));
        strncpy(newNode->lineContent, input, strlen(input));
        newNode->type = flag;
        newNode->next = NULL;
        newNode->counter = 0;

        if (*head == NULL)
        {
            *head = newNode;
        }
        else
        {
            current = *head;

            len = 0;
            if (strlen(current->label) > strlen(result))
            {
                len = strlen(current->label);
            }
            else
            {
                len = strlen(result);
            }
            if ((int)strncmp(current->label, result, len) == 0)
            {
                fprintf(stderr, "Line: %d : Label defined twice or Extern and Entry defined \n", findLineByContent(fileLines, input));
                return 3;
            }
            while (current->next != NULL)
            {
                current = current->next;
                len = 0;
                if (strlen(current->label) > strlen(result))
                {
                    len = strlen(current->label);
                }
                else
                {
                    len = strlen(result);
                }
                if ((int)strncmp(current->label, result, len) == 0)
                {
                    fprintf(stderr, "Line: %d : Label defined twice or Extern and Entry defined \n", findLineByContent(fileLines, input));


                    return 3;
                }
            }
            current->next = newNode;
        }
    }
    free(result);
    return flag;
}

int insert_label_node(struct LabelNode **head, char *label, int lineNumber, char *current_line, struct SourceFileLines *fileLines)
{
    struct LabelNode *newNode;
    struct LabelNode *current;
    int len;
    newNode = (struct LabelNode *)calloc(1, sizeof(struct LabelNode));
    newNode->lineContent = (char *)calloc(81, sizeof(char));
    if (newNode == NULL)
    {
        printf("Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    strncpy(newNode->label, label, strlen(label) - 1);
    newNode->label[strlen(label) - 1] = '\0';
    newNode->lineNumber = lineNumber;
    newNode->next = NULL;

    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        current = *head;
        while (current->next != NULL)
        {
            current = current->next;

            len = 0;
            if (strlen(current->label) > strlen(label))
            {
                len = strlen(current->label);
            }
            else
            {
                len = strlen(label) - 1;
            }
            if ((int)strncmp(current->label, label, len) == 0)
            {
                fprintf(stderr, "Line: %d : Label defined twice or Extern and Entry defined \n", findLineByContent(fileLines, current_line));

                return 1;
            }
        }
        current->next = newNode;
    }
    return 0;
}
void free_labels_list(struct LabelNode *head)
{
    struct LabelNode *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void process_file(char *file_path, struct LabelNode **headLableNode, struct LineInfo **lineInfo, struct LabelType **labelType)
{
    struct SourceFileLines *sourceFileLines;
    int flagAnalyze;
    char *file_name;
    int lineNumber;
    int ic;
    int dc;
    char line[MAX_LINE_LENGTH];
    int flagErrors;
    int labelTypeError;
    int checkForErrors;
    FILE *file;
    struct LineInfo *current;
    file_name = (char *)calloc((strlen(file_path)+strlen(".am") + 1), sizeof(char));
    strcpy(file_name, file_path);
    strcat(file_name, ".am");

    lineNumber = 100;
    ic = 0;
    dc = 0;

    file = fopen(file_name, "r");
    free(file_name);
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return;
    }

    flagErrors = 0;
    sourceFileLines = readFileAndNumberLines(file_path);

    while ((char *)fgets(line, sizeof(line), file) != NULL)
    {

        labelTypeError = insert_label_type(labelType, line, sourceFileLines);
        if (labelTypeError == 2)
        {
            flagAnalyze = 0;
            flagAnalyze = analyzeLine(lineInfo, line);
            if (flagAnalyze == 1)
            {
                continue;
            }
            current = *lineInfo;
            while ((current->next) != NULL)
            {
                current = current->next;
            }
            current->res = (char **)calloc(MAX_LINE_LENGTH, sizeof(char *)); 

            checkForErrors = validateData(*current, sourceFileLines, line);
            if (checkForErrors == 1)
            {
                flagErrors = 1;
            }
            else
            { /*  we only code a validated Line */
                fillLineFirstPass(current, &lineNumber);
                if (current->numberOfLines == 0)
                {
                    fprintf(stderr, "Line: %d : Error, %s \n", findLineByContent(sourceFileLines, current->lineContent), current->res[0]);
                }
            }
            if (current->actionFamily == 0 || current->actionFamily == 1 || current->actionFamily == 2)
            {
                ic += current->numberOfLines;
            }
            if (current->actionFamily == 3 || current->actionFamily == 4)
            {
                dc += current->numberOfLines;
            }
        }
        else if (labelTypeError == 3)
        {
            flagErrors = 1;
        }
    }
    moveNodesToEnd(lineInfo);
    current = *lineInfo;
    while (current)
    {
        char *temp1;
        char *word2;
        char *temp2;
        char *word1;
        char *word;
        int labelNodeCorrect;
        struct LabelNode *currentLabel;
        word=(char *)calloc(strlen(current->lineContent) +1 , sizeof(char));
        temp1 = (char *)calloc(strlen(current->lineContent) +1, sizeof(char));
        temp2 = (char *)calloc(strlen(current->lineContent ) +4, sizeof(char));
        strcpy(temp1, current->lineContent);
        strcpy(temp2, current->lineContent);
        word1 = (char *)calloc(strlen(current->lineContent) +1 , sizeof(char));
        word1 = strtok(temp1, " \t\n"); 
        word2 = (char *)calloc(strlen(current->lineContent ) + 1, sizeof(char));
        word2 = strtok(temp2, ":");
        strcat(word2, ":");

        if (word1 != NULL && word2 != NULL)
        {
            if (strlen(word2) < strlen(word1))
            {
                word = word2;
            }
            else
            {
                word = word1;
            }
        }

        if (word != NULL && is_valid_label(word))
        {

            labelNodeCorrect = insert_label_node(headLableNode, word, current->startLine, line, sourceFileLines);
            if (labelNodeCorrect == 1)
            {
                flagErrors = 1;
            }

            currentLabel = *headLableNode;
            while (currentLabel->next != NULL)
            {
                currentLabel = currentLabel->next;
            }
            currentLabel->lineContent = line;
        }
        current = current->next;
    }

    
    /*  validate labels */
    check_labels(*headLableNode, sourceFileLines);
    /* validate all extern and entry labels */
    checkLabelsType(*headLableNode, *labelType, sourceFileLines);
    /* coding the rest of the lines (labels) */

    if (flagErrors == 0)
    {
        secondPass(*lineInfo, headLableNode, labelType);
        printResForObject(*lineInfo, ic, dc, file_path);
        exportFiles(*labelType, file_path);
    }
    fclose(file);
    free(sourceFileLines);
}

