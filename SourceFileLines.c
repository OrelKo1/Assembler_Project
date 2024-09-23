#include "SourceFileLines.h"

struct LinexIndex
{
    int lineNumber;
    char *line;
};

struct SourceFileLines
{
    int totalLines;
    struct LinexIndex lines[2000];
};

 SourceFileLines readFileAndNumberLines(const char *filename)
{
    struct LinexIndex *lineInfo;
    struct SourceFileLines *fileLines;
    char *open_file;
    int totalLines;
    char ch;
    int lineNumber;
    char buffer[256];
    int lineIndex;
    FILE *file;
    open_file = (char *)calloc((strlen(filename) + 4), sizeof(char));
    strcpy(open_file, filename);
    strcat(open_file, ".as");

    file = fopen(open_file, "r");
    free(open_file);
    if (file == NULL)
    {
        perror("Failed to open file");
        return NULL;
    }

    totalLines = 0;

    while ((ch = fgetc(file)) != EOF)
    {
        if (ch == '\n')
        {
            totalLines++;
        }
    }
    /* // Account for the last line if it doesn't end with a newline */
    if (ftell(file) > 0)
    {
        totalLines++;
    }
    rewind(file);

    fileLines = (struct SourceFileLines *)calloc(sizeof(struct SourceFileLines) + sizeof(struct LinexIndex), totalLines);
    if (fileLines == NULL)
    {
        fclose(file);
        perror("Memory allocation failed");
        return NULL;
    }

    fileLines->totalLines = totalLines;

    lineNumber = 1;

    lineIndex = 0;
    while (fgets(buffer, sizeof(buffer), file))
    {

        lineInfo = &fileLines->lines[lineIndex];
        lineInfo->lineNumber = lineNumber++;
        lineInfo->line  = (char*) calloc(strlen(buffer) + 1,sizeof(char));
	strcpy(lineInfo->line,buffer);

        lineIndex++;
    }

    fclose(file);
    return fileLines;
}


int findLineByContent(const  SourceFileLines fileLines, const char *searchLine)
{
    int i;
    for (i = 0; i < fileLines->totalLines; i++)
    {
        if(fileLines->lines[i].line!=NULL){
        if (strcmp(fileLines->lines[i].line, searchLine) == 0)
        {
            return fileLines->lines[i].lineNumber;
        }
        }
        
    }
    return -1;
}
