#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

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

    fclose(input);
    fclose(output);

    return count;
}

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
