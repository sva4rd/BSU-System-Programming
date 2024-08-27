#ifndef REPLACING_H
#define REPLACING_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

int process_file(FILE* input, FILE* output, char replaceChar);

int replaceDigitsInFile(const char* inputFile, const char* outputFile, char replaceChar);

#ifdef __cplusplus
}
#endif

#endif /* REPLACING_H */
