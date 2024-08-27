#include <stdio.h>
#include <ctype.h>
#include "../include/replacing.h"

int replaceDigitsInFile(const char* inputFile, const char* outputFile, char replaceChar) {
    FILE* input = fopen(inputFile, "r");
    if (input == NULL) {
        perror("Ошибка при открытии входного файла");
        return -1;
    }
    
    FILE* output = fopen(outputFile, "w");
    if (output == NULL) {
        perror("Ошибка при открытии выходного файла");
        fclose(input);
        return -1;
    }
    
    int count = process_file(input, output, replaceChar);
    
    fclose(input);
    fclose(output);
    
    return count;
}
