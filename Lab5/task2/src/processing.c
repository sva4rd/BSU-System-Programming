#include <stdio.h>
#include <ctype.h>
#include "../include/processing.h"

int process_file(FILE* input, FILE* output, char replaceChar) {
    int count = 0;
    int ch;
    
    while ((ch = fgetc(input)) != EOF) {
        if (isdigit(ch)) {
            fputc(replaceChar, output);
            count++;
        } else {
            fputc(ch, output);
        }
    }
    
    return count;
}
