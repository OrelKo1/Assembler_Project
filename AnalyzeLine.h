#ifndef _ANALYZE_LINE_H
#define _ANALYZE_LINE_H
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "HeaderLibraries.h"
#include "Defenitions.h"


typedef struct LineInfo *LineInfo;
typedef struct LineInfo
{
    int actionCode;
    int actionFamily;
    int sourceAddress;
    int destinationAddress;
    char *srcValue;
    char *desValue;
    int ARE;
    char **res;
    char *lineContent;
    int startLine;
    int wasMoved;
    int numberOfLines;
    LineInfo next;
}lineInfo;

/* checks the string for unwanted chars like \r \n , spaces, commas etc */
void CleanString(const char *string, char *empty);
/* in case of data or string extract the correct section to info */
char *extractDataStringSection(const char *input, int teller);
/* checks if the section is all integers */
bool isInteger(char str[10]);
/* finds the source opreand value */
char *findSourceValue(char *string, int addressType);
/* check for a declare of data or string */
int checkDataStringDeclaration(const char *word1, const char *word2);
/* check if the string is in limits */
bool isStringWithinLimit(const char *str);
/* returns the action number from the data chart */
int findActionNumber(const char *input);
/* checks if a label is valid (first check, another deep check in validateData function) */
bool isValidLabel(const char *str);
/* returns the addressing method of the opreand */
int findAddressingMethod(char opreand[31]);
/* analyzes the line and calls all the other functions */
int analyzeLine(LineInfo *head, const char *line);
/* checks if the string is empty */
bool isEmptyLine(const char *str);
/* checks if the char is white space */
bool isWhitespace(char c);

#endif