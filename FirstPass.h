#ifndef _FIRST_PASS_H
#define _FIRST_PASS_H
#include "SecondPass.h"
#include "HeaderLibraries.h"
#include <stdlib.h>

/* free desvalue of the node */
void freeNodeDesValue(LineInfo node);
/* free source value of the node */
void freeNodeSrcValue(LineInfo node);
/*gets a string representing a binary value and returns its result plus one */
char* addOneToBinary(const char* binary);
/*gets a string representing a binary value and for each '0' replace it with '1' ,
and for each '1' replace it with '0'*/
void flip_binary_string(char *binary_string);
/*gets a string ans returns all chars that betwwen " "*/
char *extractBetweenApostrophes(const char *input);
/*gets a string contains numbers and returns array of ints*/
int* parseNumbers(const char* input, int* size);
/*coding binary value for an instruction line with src and des operands  */
void buildTwoOperandCodeLine(char *res,  LineInfo lineInfo);
/*coding binary value for an instruction line with des operand  */
void buildOneOperandCodeLine(char *res,  LineInfo lineInfo);
/*coding binary value for an instruction line with no operands  */
void buildNoOperandCodeLine(char *res,  LineInfo lineInfo);
/*coding binary value for an instruction line with 2 Registers as operands  */
void build2RegisterString(char *res, char *sourceOperand, char *destinationOperand);
/*coding binary value for an instruction line with src Register as operands  */
void buildSourceRegisterString(char *res, char *sourceOperand);
/*coding binary value for an instruction line with des Register as operands  */
void buildDestinationRegisterString(char *res, char *destinationOperand);
/*coding binary value for data line - numbers */
void buildNumberData(char **res, char *dataAsString ,int *line) ;
/*returns AsciiCode As Long*/
long getAsciiCodeAsLong(char inputChar);
/*coding binary value for data line - string */
void buildStringData(char **res, char *string);
/*coding binary value for an instruction line with number as operand */
void buildNumberString(char *res, char  *number);
/*for each line from am file - culculats the binary code that should be generated */
void fillLineFirstPass( LineInfo lineInfo , int *line);
#endif