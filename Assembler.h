#ifndef _ASSEMBLER_H
#define _ASSEMBLER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ValidateEntryAndExtern.h"
#include "SourceFileLines.h"
#include "AnalyzeLine.h"
#include "PreAssembler.h"
#include "FirstPass.h"
#include "Defenitions.h"
#include "HeaderLibraries.h"
#include "AnalyzeLine.h"

/*create ent and ext files*/
void exportFiles(LabelType head,char *original_file_name);
/*create ob file (base 64)*/
void printResForObject( LineInfo lineInfo , int ic, int dc,char *original_file_name);
/*making sure data section comes after  instructions */
void moveNodesToEnd( LineInfo* head);
/*validate labels are according to requirements */
int is_valid_label(const char *word);
/*index labels defined as entry or extern */
int insert_label_type( LabelType *head, const char *input,  SourceFileLines fileLines);
/*index labels line*/
int insert_label_node( LabelNode *head, char *label, int lineNumber, char *current_line,  SourceFileLines fileLines);
/* free the labels list */
void free_labels_list( LabelNode head);
/*goes over the file and applying lgic in a loop */
void process_file(char *file_path,  LabelNode* headLableNode ,  LineInfo* lineInfo ,  LabelType* labelType);
#endif