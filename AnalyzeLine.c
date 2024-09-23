#include "AnalyzeLine.h"

struct Data
{
    int actionCode;
    char actionName[6];
};
struct Data actionTable[] = {
    {0, "mov"},
    {1, "cmp"},
    {2, "add"},
    {3, "sub"},
    {4, "not"},
    {5, "clr"},
    {6, "lea"},
    {7, "inc"},
    {8, "dec"},
    {9, "jmp"},
    {10, "bne"},
    {11, "red"},
    {12, "prn"},
    {13, "jsr"},
    {14, "rts"},
    {15, "stop"}};

bool isWhitespace(char c)
{
    return c == ' ' || c == '\t';
}

bool isEmptyLine(const char *str)
{

    size_t len;
    size_t i;
    if (str == NULL)
    {
        return true;
    }
    len = strlen(str);

    if (len == 1 && str[0] == '\n')
    {
        return true;
    }

    for (i = 0; i < len; i++)
    {
        if (!isWhitespace(str[i]))
        {
            return false;
        }
    }

    return true;
}
void CleanString(const char *string, char *empty)
{
    
    int start;
    int end;
    start = 0;
    while (string[start] == ',' || string[start] == '\n' || string[start] == '\t' || string[start] == ' ')
    {
        start++;
    }
    end = strlen(string);
    while (end >=1 && ( string[end - 1] == ',' || string[end - 1] == '\n' || string[end - 1] == '\t' || string[end - 1] == ' '||string[end - 1] == '\r'))
    {
        end--;
    }

        if((end-start)>=0){

    strncpy(empty, string + start, end - start);
    }
    else strcpy(empty,"");
    
    return;
}

char *extractDataStringSection(const char *input, int teller) 
{
    char *dataKeyword; /* litreal string */
    char *dataPos; /* a pointer to a litreal string  */
    char *dataSection;
    int start;
    dataSection=(char*)calloc(strlen(input),sizeof(char));
    start=0;
    switch (teller)
    {
    case 3:
        dataKeyword=".string";
        break;
    case 4:
        dataKeyword=".data";
        break;
    case 5:
        dataKeyword=".entry";
        break;
    case 6:
        dataKeyword=".extern";
        break;

    default:
        free(dataSection);
        return NULL;
    }
    dataPos = strstr(input, dataKeyword);
    

    if (dataPos)
    {

        size_t dataPosIndex;
        size_t dataSectionLength;
        char temp[MAX_LINE_LENGTH];
        

        dataPosIndex = dataPos - input;
        dataSectionLength = strlen(input) - dataPosIndex - strlen(dataKeyword);

        if (dataSection)
        {

            strncpy(temp, input + dataPosIndex + strlen(dataKeyword), dataSectionLength);
            while(temp[start]==' '){
                start++;
            }
            temp[dataSectionLength - 1+start] = '\0'; 
            strcpy(dataSection,temp+start);
            

            return dataSection;
        }
        else
        {
            fprintf(stderr, "Memory allocation failed in the file 'extractDataSection\n");

            free(dataSection);
            return NULL;
        }
    }
    else
    {

        free(dataSection);
        return NULL; /* ".data" not found in the input string */
    }
}

bool isInteger(char str[10])
{
    int i;
    /*  Check if the string is empty */
    if (str[0] == '\0')
    {
        return false;
    }

    /* Check for a potential sign at the beginning */
    i = 0;
    if (str[0] == '+' || str[0] == '-')
    {
        i = 1;

        if (str[i] == '\0')
        {
            return false;
        }
    }

    /*  Check each character from the starting position */
    while (str[i] != '\0')
    {

        if (!isdigit(str[i]) && str[i] != '.')
        {
            return false;
        }
        i++;
    }

    return true;
}

char *findSourceValue(char *string, int addressType)
{
    char *digit = NULL; 
    char *copy = NULL; 
    int i;

    if (addressType == -1)
    {
        return NULL;
    }
    else if (addressType == 5)
    {
        for (i = 0; string[i] != '\0'; i++)
        {
            if (isdigit(string[i]))
            {
                digit = (char *)calloc(2 , sizeof(char)); 
                if (digit == NULL)
                {
                    fprintf(stderr, "Memory allocation error in findSourceValue\n");
                    exit(EXIT_FAILURE);
                }
                digit[0] = string[i];
                digit[1] = '\0';
                return digit;
            }
        }
        return NULL; 
    }
    else
    {
        copy = (char *)calloc(strlen(string) + 1, sizeof(char));
        if (copy == NULL)
        {
            fprintf(stderr, "Memory allocation error in findSourceValue\n");
            exit(EXIT_FAILURE);
        }
        CleanString(string, copy);
        return copy;
    }
}

int checkDataStringDeclaration(const char *word1, const char *word2)
{

    /* Check if either the first or second word is ".data" or ".string" */
    if (strcmp(word1, ".data") == 0 || strcmp(word2, ".data") == 0)
    {
        return 4;
    }
    else if ((strcmp(word1, ".string") == 0 || strcmp(word2, ".string") == 0))
    {
        return 3;
    }
    else if ((strcmp(word1, ".entry") == 0 || strcmp(word2, ".entry") == 0))
    {
        return 5;
    }
    else if ((strcmp(word1, ".extern") == 0 || strcmp(word2, ".extern") == 0))
    {
        return 6;
    }

    return -1;
}
bool isStringWithinLimit(const char *str)
{
    int length;
    int i;
    length = 0;
    for (i = 0; str[i] != '\0'; i++)
    {
        if (str[i] != '\n')
        {
            length++;
        }
    }

    return length <= 80;
}

int findActionNumber(const char *input)
{

    int numActions;
    int i;
    numActions = sizeof(actionTable) / sizeof(actionTable[0]);

    for (i = 0; i < numActions; i++)
    {
        if (strcmp(input, actionTable[i].actionName) == 0)
        {
            return actionTable[i].actionCode;
        }
    }

    return -1;
}
bool isValidLabel(const char *str)
{

    if (strlen(str) == 0)
    {
        return false;
    }

    if (isalpha(str[0]))
    {
        return true;
    }
    return false;
}
int findAddressingMethod(char opreand[31])
{
    char *copiedString;
    int index;
    index =-1;
    copiedString = (char *)calloc(strlen(opreand) + 1, sizeof(char));
    CleanString(opreand, copiedString);

    if (copiedString[0] == '@')
    {
        
        index= 5;
    }
    else if (isInteger(copiedString))
    {
        
        index= 1;
    }
    else if (isValidLabel(copiedString))
    {
       
        index= 3;
    }
    free(copiedString);
    return index;
}

char **splitWords(const char *input, int *numWords)
{
    char **words;
    char *tempInput;
    char *token;

    words = (char **)calloc(MAX_LINE_LENGTH, sizeof(char *)); 
    if (words == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    tempInput = (char *)calloc(strlen(input) + 1, sizeof(char));
    strcpy(tempInput, input);
    if (tempInput == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    token = strtok(tempInput, " :,");

    *numWords = 0;
    while (token != NULL)
    {
        words[*numWords] = (char *)calloc(strlen(token) + 1, sizeof(char));
        CleanString(token, words[*numWords]);
        
        if (words[*numWords] == NULL)
        {
            free(tempInput);
            tempInput=NULL;
            free(token);
            token=NULL;
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
        (*numWords)++;
        token = strtok(NULL, " :,");
    }
    free(tempInput);
    free(token);
    token=NULL;
    tempInput=NULL;
    return words;
}

int analyzeLine(LineInfo *head, const char *line)
{
    int temp2;
    int i;
    char **words;
    char *checker; 
    int numWords;
    int DataOrString;
   

    char *destAddress;
    char *srcAddress;
    LineInfo addressInfo;
    checker = (char *)calloc(strlen(line) + 1, sizeof(char));
    CleanString(line, checker);
    if (checker == NULL)
    {
        return 1;
    }
    if (checker[0] == ';') /*  check for comment */
    {
        free(checker);
        return 1;
    }

    if (isEmptyLine(checker)) /*  check for an empty line */
    {
        free(checker);
        return 1;
    }
    free(checker);
    addressInfo = (LineInfo)calloc(1, sizeof(struct LineInfo));
    addressInfo->lineContent = (char *)calloc(strlen(line) + 1,sizeof(char));
    strcpy(addressInfo->lineContent, line);
    addressInfo->lineContent[strlen(line)] = '\0';
    addressInfo->actionCode = -1;
    addressInfo->actionFamily = -1;
    addressInfo->sourceAddress = -1;
    addressInfo->destinationAddress = -1;
    addressInfo->srcValue = NULL;
    addressInfo->desValue = NULL;
    addressInfo->ARE = -1;
    addressInfo->wasMoved = 0;
    addressInfo->numberOfLines = 0;
    addressInfo->next = NULL;

    DataOrString = -1;

    words = splitWords(line, &numWords);
    if (numWords >= 2)
    {
      
        DataOrString = checkDataStringDeclaration(words[0], words[1]);
    }
    if (DataOrString == 3 || DataOrString == 4 || DataOrString == 5 || DataOrString == 6)
    {
        addressInfo->actionFamily = DataOrString;
        addressInfo->desValue = extractDataStringSection(line, DataOrString);
    }
    else
    {
    
        destAddress = (char *)calloc(31, sizeof(char));
        srcAddress = (char *)calloc(31, sizeof(char));
 
        temp2 = findActionNumber(words[0]);
        if (temp2 == -1) /*  now we know we have a label */
        {
            if (words[1] == NULL)
            {
                if (*words != NULL)
                {
                    for (i = 0; i < numWords; i++)
                    {
                        free(words[i]);
                        words[i]=NULL;
                    }
                }
                free(words);
                words=NULL;
                free(destAddress);
                free(srcAddress);
                return 1;
            }

            addressInfo->actionCode = findActionNumber(words[1]);
            if (words[2] != NULL)
            {
                if (words[3] != NULL)
                {
                    strcpy(srcAddress, words[2]);
                    strcpy(destAddress, words[3]);
                }
                else
                {
                    strcpy(destAddress, words[2]);
                }
            }
        }
        else
        { /* // now we know we dont have a label */
            addressInfo->actionCode = temp2;

            if (words[1] != NULL)
            {
                if (words[2] != NULL)
                {
                    strcpy(destAddress, words[2]);
                    strcpy(srcAddress, words[1]);
                }
                else
                {
                    strcpy(destAddress, words[1]);
                }
            }
        }
        if (addressInfo->actionCode == -1)
        {
        }
        else
        {

            if (destAddress[0] == '\0') /* check number of opreands to determine family, and if only 1 opreand set it to src */
            {
                if (srcAddress[0] == '\0')
                {
                    addressInfo->actionFamily = 2;
                }
                else
                {
                    strcpy(destAddress, srcAddress);
                    strcpy(srcAddress, "");
                    addressInfo->actionFamily = 1;
                }
            }
            else if (srcAddress[0] == '\0')
            {
                addressInfo->actionFamily = 1;
            }
            else
            {
                addressInfo->actionFamily = 0;
            }
            addressInfo->destinationAddress = findAddressingMethod(destAddress);
            
            addressInfo->sourceAddress = findAddressingMethod(srcAddress);
            addressInfo->srcValue = findSourceValue(srcAddress, addressInfo->sourceAddress);
            addressInfo->desValue = findSourceValue(destAddress, addressInfo->destinationAddress);
            addressInfo->ARE = 0;
        }
        free(destAddress);
        free(srcAddress);
    }

    if (*head == NULL)
    {
        *head = addressInfo;
    }
    else
    {
        LineInfo current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = addressInfo;
    }
    if (*words != NULL)
    {
        for (i = 0; i < numWords; i++)
        {
            free(words[i]);
            words[i]=NULL;
        }
    }
    free(words);
    words=NULL;
    
    
    return 0;
}
