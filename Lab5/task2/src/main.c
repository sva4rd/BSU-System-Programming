#include <stdio.h>
#include <ctype.h>
#include "../include/replacing.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Использование: %s <входной_файл> <выходной_файл> <символ>\n", argv[0]);
        return 1;
    }
    
    const char* inputFile = argv[1];
    const char* outputFile = argv[2];
    char replaceChar = argv[3][0]; // Берем первый символ из третьего аргумента командной строки
    
    int result = replaceDigitsInFile(inputFile, outputFile, replaceChar);
    if (result == -1) {
        printf("Произошла ошибка при выполнении программы.\n");
        return 1;
    }
    
    printf("Операции выполнены успешно. Количество замененных цифр: %d\n", result);
    return 0;
}
