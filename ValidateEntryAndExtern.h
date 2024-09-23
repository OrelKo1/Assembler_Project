#ifndef _VALIDATE_ENTRY_AND_EXTERN_H
#define _VALIDATE_ENTRY_AND_EXTERN_H

#include <stdio.h>
#include <string.h>
#include "Defenitions.h"
#include <stdbool.h>
#include "ValidateData.h"
#include "HeaderLibraries.h"

typedef struct LabelNode *LabelNode;
typedef struct LabelType *LabelType;
typedef struct LabelNode
{
    char label[MAX_LABEL_LENGTH];
    int lineNumber;
    struct LabelNode *next;
    char *lineContent;
}labelNode;
typedef struct LabelType
{
    char label[MAX_LABEL_LENGTH];
    int type; /*  // 0 for entry, 1 for external */
    int lines[MAX_LABEL_LENGTH];
    int counter;
    struct LabelType * next;
    char *lineContent;
}labelType;

bool is_valid_instruction(const char *label);
void check_labels( LabelNode head,  SourceFileLines fileLines);
void checkLabelsType( LabelNode labelNodeHead,  LabelType labelTypeHead,  SourceFileLines fileLines);
char* getLabelTypeLabel(LabelType labelType);
int getLabelTypeType(LabelType labelType);
int* getLabelTypeLines(LabelType labelType);
int getLabelTypeCounter(LabelType labelType);
LabelType getLabelTypeNext(LabelType labelType);
char *getLabelTypeLineContent(LabelType labelType);

void setLabelTypeLabel(LabelType labelType ,char* label);
void setLabelTypeType(LabelType labelType,int type);
void setLabelTypeLines(LabelType labelType,int* lines);
void setLabelTypeCounter(LabelType labelType,int counter);
void setLabelTypeNext(LabelType labelType,LabelType bext);
void setLabelTypeLineContent(LabelType labelType,char * Content);




char* getLabelNodeLabel(LabelNode labelNode);
int getLabelNodeLineNumber(LabelNode labelNode);
LabelType getLabelNodeNext(LabelNode labelNode);
char *getLabelTypeLineContent(LabelType labelType);
#endif