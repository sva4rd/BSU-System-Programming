#ifndef REPLACING_H
#define REPLACING_H

#include <stdio.h>

int replaceDigitsInFile(const char* inputFile, const char* outputFile, char replaceChar);
int process_file(FILE* input, FILE* output, char replaceChar);

#endif
