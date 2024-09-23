#include "ValidateEntryAndExtern.h"




/*for each label checks that it is not an action or register name*/

bool is_valid_instruction(const char *label)
{
    int i;
    const char *invalid_instructions[] = {
        "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc",
        "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop",
        "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "data", "entry", "extern"};

    int num_invalid_instructions;
    num_invalid_instructions = sizeof(invalid_instructions) / sizeof(invalid_instructions[0]);

    for (i = 0; i < num_invalid_instructions; i++)
    {
        if (strcmp(label, invalid_instructions[i]) == 0)
        {
            return false;
        }
    }

    return true;
}
/*validates label  according to requirements */

void check_labels( LabelNode head,  SourceFileLines fileLines)
{
    char label_copy[MAX_LABEL_LENGTH];
    bool validFormat;
    LabelNode current;
    current = head;
    while (current != NULL)
    {
        char *label;
        strcpy(label_copy, current->label);

        label = label_copy;

        if (strlen(current->label) > MAX_LABEL_LENGTH)
        {
            fprintf(stderr, "Line: %d : Error, label length exceeds maximum limit \n", findLineByContent(fileLines, current->lineContent));
        }

        if (!isalpha(label[0]))
        {
            fprintf(stderr, "Line: %d : Error, invalid label format, label doesn't start with a letter \n", findLineByContent(fileLines, current->lineContent));
        }
        else
        {
            int i;
            validFormat = true;
	        i = 1;
            for (i = 1; label[i] != '\0'; i++)
            {
                if (!isalnum(label[i]) && label[i] != ':')
                {
                    fprintf(stderr, "Line: %d : Error, invalid label format , invallid sign \n", findLineByContent(fileLines, current->lineContent));
                    validFormat = false;
                    break;
                }
            }

            if (validFormat)
            {

                label[strlen(label)] = '\0';

                if (!is_valid_instruction(label))
                {
                    fprintf(stderr, "Line: %d : Error, illegal label , instruction , register or an action name \n", findLineByContent(fileLines, current->lineContent));
                }
            }
        }

        current = current->next;
    }
}
/*validates label defined as entry - are defined in the as file, and for extern that it is not defined in the as file*/

void checkLabelsType(LabelNode labelNodeHead, LabelType labelTypeHead,  SourceFileLines fileLines)
{
    LabelType labelTypePtr;
    char *copy_label;
    labelTypePtr = labelTypeHead;
    

    while (labelTypePtr != NULL)
    {
         LabelNode labelNodePtr = labelNodeHead;
        

        while (labelNodePtr != NULL)
        {
            copy_label = (char *)calloc(strlen(labelNodePtr->label) + 1, sizeof(char));
            strcpy(copy_label, labelNodePtr->label);
            if ((labelTypePtr->type == 0 && strcmp(copy_label, labelTypePtr->label) == 0) ||
                (labelTypePtr->type == 1 && strcmp(copy_label, labelTypePtr->label) == 0))
            {
                if (labelTypePtr->type == 0)
                {
                    labelTypePtr->lines[0] = labelNodePtr->lineNumber;
                }
                else if (labelTypePtr->type == 1)
                {
                    
                    fprintf(stderr, "Line: %d : Error, extern but label found %s \n", findLineByContent(fileLines, labelTypePtr->lineContent), labelTypePtr->label);
                }
                break;
            }
            if (labelNodePtr->next == NULL)
            {
                if (labelTypePtr->type == 0)
                {
                    free(copy_label);
                    fprintf(stderr, "Line: %d : Error, entry but label not found  %s \n", findLineByContent(fileLines, labelTypePtr->lineContent), labelTypePtr->label);
                    break;
                }
                if (labelTypePtr->type == 1)
                {

                    break;
                }
            }

            labelNodePtr = labelNodePtr->next;
        }
        if (labelTypePtr->next == NULL)
        {
            free(copy_label);
            return;
        }

        labelTypePtr = labelTypePtr->next;
        free(copy_label);
    }
}
