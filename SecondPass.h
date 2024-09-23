#ifndef _SECOND_PASS_H
#define _SECOND_PASS_H
#include "ValidateEntryAndExtern.h"

/*returns a string  representing he binary value of an int */
char* int_to_binary(long num);
/*takes a string and copletes its size to match the size param by adding '0'*/
void complete_string_size(char *input_string, int size);
/*returns the line number for a givven label*/
int get_label_line_number_decimal( LabelNode head, const char* line);
/*returns if a label defined as extern and also updates for the existing label to be used for the ext file*/
int get_labelType_line_number_decimal( LabelType head, const char* line , int lineNumber);
/*coding binary value for a line containes label as opernad*/
void buildLabelString(char *res, char *label);
/*coding lines in a loop for instructions that contains labels */
void secondPass( LineInfo lineInfo, LabelNode* head ,  LabelType *labelType);
#endif