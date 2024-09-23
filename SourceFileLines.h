#ifndef _SOURCE_FILE_LINES_H
#define _SOURCE_FILE_LINES_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HeaderLibraries.h"

typedef struct LinexIndex *LinexIndex;
typedef struct SourceFileLines *SourceFileLines;
/*indexing each line from as file*/
SourceFileLines readFileAndNumberLines(const char *filename);
/*returns the line number for a line as it in as file*/
int findLineByContent(const  SourceFileLines fileLines, const char *searchLine);
#endif