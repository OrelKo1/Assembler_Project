#include "ValidateData.h"






struct Data2
{
    int actionCode;
    char actionName[5];
    int validSourceMethod[3];
    int validDestinationMethod[3];
    int options_for_source;
    int options_for_destination;
};

struct Data2 actionTable2[] = {
    {0, "mov", {1, 3, 5}, {3, 5}, 3, 2},
    {1, "cmp", {1, 3, 5}, {1, 3, 5}, 3, 3},
    {2, "add", {1, 3, 5}, {3, 5}, 3, 2},
    {3, "sub", {1, 3, 5}, {3, 5}, 3, 2},
    {4, "not", {-1}, {3, 5}, 0, 2},
    {5, "clr", {-1}, {3, 5}, 0, 2},
    {6, "lea", {3}, {3, 5}, 1, 2},
    {7, "inc", {-1}, {3, 5}, 0, 2},
    {8, "dec", {-1}, {3, 5}, 0, 2},
    {9, "jmp", {-1}, {3, 5}, 0, 2},
    {10, "bne", {-1}, {3, 5}, 0, 2},
    {11, "red", {-1}, {3, 5}, 0, 2},
    {12, "prn", {-1}, {1, 3, 5}, 0, 3},
    {13, "jsr", {-1}, {3, 5}, 0, 2},
    {14, "rts", {-1}, {-1}, 0, 0},
    {15, "stop", {-1}, {-1}, 0, 0}};

 
bool isValidString(const char *input) {
    bool seenComma = false;
    int i;
    int length;
    length = strlen(input);
    

    for (i = 0; i < length; i++) {
        if (input[i] == ',') {
            if (!seenComma) {
                seenComma = true;
            } else {
               return false;; 
            }
        } else if (!isspace(input[i])) {
            seenComma = false;
        }
    }
    return true;
}




int validateData(struct LineInfo data, struct SourceFileLines *fileLines, char *line)
{
    char *checker;
    int i;
    checker = (char *)calloc(strlen(line) + 1, sizeof(char));
    CleanString(line, checker);
    if (checker == NULL)
    {
        return 0;
    }
    if (checker[0] == ';') 
    {
        free(checker);
        return 0;
    }

    if (isEmptyLine(checker)) /* check for an empty line */
    {
        free(checker);
        return 0;
    }
    if (!isValidString(line))
    {
        fprintf(stderr, "Line: %d : Error, two or more commas in a row \n", findLineByContent(fileLines, line));
        return 1;
    }
    
    if (data.actionFamily == 5 || data.actionFamily == 6)
    {
        free(checker);
        return 0;
    }
    free(checker);
    if (data.destinationAddress == 3)
    {
        check_label_in_opreand(data.desValue, fileLines, line);
    }

    if (data.actionFamily == -1) /*  check for illegal action or instruction */
    {
        fprintf(stderr, "Line: %d : Error, line illegal, no legal action or instruction \n", findLineByContent(fileLines, line));
        return 1;
    }
    if (data.actionFamily == 4)
    {
        if (!areAllIntegers(data.desValue))
        {
            fprintf(stderr, "Line: %d : Error, .data values are not integers \n", findLineByContent(fileLines, line));
            return 1;
        }
        else
        {

            return 0;
        }
    }
    if(data.actionFamily == 3) {
        return 0;
    }
    /*  if no source opreand legal for the action but the line contains a source opreand */
    if (actionTable2[data.actionCode].options_for_source == 0 && data.sourceAddress != -1)
    {
        fprintf(stderr, "Line: %d : Error, Action not supporting source opreand \n", findLineByContent(fileLines, line));
        return 1;
    }
    /*  Making sure the source opreand is legal for the action */
    for (i = 0; i < actionTable2[data.actionCode].options_for_source; i++)
    {
        if (data.sourceAddress == actionTable2[data.actionCode].validSourceMethod[i])
        {

            break;
        }
        else if (i == actionTable2[data.actionCode].options_for_source - 1) /* if we are out of options without a match */
        {
            fprintf(stderr, "Line: %d : Error, Source Address Method illegal \n", findLineByContent(fileLines, line));
            return 1;
        }
    }
    /*  if the source is a register, checking that its value is between 0 and 7 */
    if (data.sourceAddress == 5 && (atoi(data.srcValue) < 0 || atoi(data.srcValue) > 7))
    {
        fprintf(stderr, "Line: %d : Error, illegal source register number \n", findLineByContent(fileLines, line));

        return 1;
    }
    /*     if the source is a number , checking that its value is an integer and not a float */
    if (data.sourceAddress == 1)
    {
        if (!areAllIntegers(data.srcValue))
        {
            fprintf(stderr, "Line: %d : Error, source value is not an integer \n", findLineByContent(fileLines, line));
            return 1;
        }
    }

    /*  Making sure the destination opreand is legal for the action */
    if (actionTable2[data.actionCode].options_for_destination == 0 && data.destinationAddress != -1)
    {
        fprintf(stderr, "Line: %d : Error, Action not supporting destination opreand \n", findLineByContent(fileLines, line));

        return 1;
    }
    /*  Making sure the source opreand is legal for the action */
    for (i = 0; i < actionTable2[data.actionCode].options_for_destination; i++)
    {
        if (data.destinationAddress == actionTable2[data.actionCode].validDestinationMethod[i])
        {

            break;
        }
        else if (i == actionTable2[data.actionCode].options_for_destination - 1) /*  if we are out of options without a match */
        {
            fprintf(stderr, "Line: %d : Error, Destination Address Method illegal \n", findLineByContent(fileLines, line));

            return 1;
        }
    }
    if (data.destinationAddress == 5 && (atoi(data.desValue) < 0 || atoi(data.desValue) > 7))
    {
        fprintf(stderr, "Line: %d : Error, illegal destination register number \n", findLineByContent(fileLines, line));

        return 1;
    }
    /* if the destination is a number , checking that its value is an integer and not a float */
    if (data.destinationAddress == 1)
    {
        if (!areAllIntegers(data.desValue))
        {
            fprintf(stderr, "Line: %d : Error, destination value is not an integer \n", findLineByContent(fileLines, line));
            return 1;
        }
    }
    return 0;
}

void check_label_in_opreand(char *label, struct SourceFileLines *fileLines, char *current_line)
{
    char label_copy[MAX_LABEL_LENGTH];
    int i;
    strcpy(label_copy, label);

    if (strlen(label_copy) > MAX_LABEL_LENGTH)
    {
        fprintf(stderr, "Line: %d : Error, label length exceeds maximum limit \n", findLineByContent(fileLines, current_line));
    }

    if (!isalpha(label_copy[0]))
    {
        fprintf(stderr, "Line: %d : Error, invalid label format, label doesn't start with a letter \n", findLineByContent(fileLines, current_line));
    }
    else
    {
	
        for (i = 1; label[i] != '\0'; i++)
        {
            if (!isalnum(label_copy[i]) && label[i] != ':')
            {
                fprintf(stderr, "Line: %d : Error, invalid label format , invallid sign \n", findLineByContent(fileLines, current_line));
                break;
            }
        }
    }
}


bool areAllIntegers(const char *input)
{

    size_t length;
    
    char *copy;
    char c;
    bool parsingNumber;
    int start;
    size_t i;
    start =0;
    copy = (char *)calloc(strlen(input) + 1, sizeof(char));
    if (copy == NULL)
    {
        fprintf(stderr, "Memory allocation error in sourceValue file\n");
        exit(EXIT_FAILURE);
    }
    CleanString(input, copy);
    length = strlen(copy);

    parsingNumber = false;
    while (input[start]==' '){ /* skipping empty notes */
        start++;
    }
    if (input[start]==',') /* checking if the first note before any integer is a comma */
    {
        free(copy);
        return parsingNumber;
    }
    
    

    for (i = 0; i < length; i++)
    {
        c = copy[i];

        /*   If the current character is a digit or a sign of a number */
        if (isdigit(c) || (c == '-' && !parsingNumber) || (c == '+' && !parsingNumber) || (c == '\0' && i == strlen(copy)))
        {
            parsingNumber = true; /* We are parsing a number*/
        }
        /*   If the current character is a comma or whitespace */
        else if (c == ',' || isspace(c))
        {
            parsingNumber = false;
        }
        /*  If the current character is not a digit, sign, comma, or whitespace */
        else
        {
            return false;
        }
    }
    free(copy);
    return parsingNumber;
}
