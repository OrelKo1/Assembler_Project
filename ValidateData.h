#ifndef _VALIDATE_DATA_H
#define _VALIDATE_DATA_H
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "AnalyzeLine.h"
#include "SourceFileLines.h"
#include "HeaderLibraries.h"
#include "Defenitions.h"

typedef struct Data2 *Data2;
/*for each line validates the action , operand and data according to requirements */
int validateData(struct LineInfo data,struct SourceFileLines *fileLines, char *line);
bool isWhitespace(char c);
bool isEmptyLine(const char *str);
/*checking that string contains only chars that are representing numbers */
bool areAllIntegers(const char *input);
/* a function to check for two commas in a row */
bool isValidString(const char *input);
void check_label_in_opreand(char *label, struct SourceFileLines *fileLines, char *current_line);
#endif