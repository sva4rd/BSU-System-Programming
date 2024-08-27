#include <stdio.h>
#include <ctype.h>
#include <dlfcn.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Использование: %s <входной_файл> <выходной_файл> <символ>\n", argv[0]);
        return 1;
    }

    const char* inputFile = argv[1];
    const char* outputFile = argv[2];
    char replaceChar = argv[3][0];

    void* handle = dlopen(".libs/libreplacing.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Ошибка при загрузке библиотеки: %s\n", dlerror());
        return 1;
    }

    int (*replaceDigitsInFile)(const char*, const char*, char);
    replaceDigitsInFile = dlsym(handle, "replaceDigitsInFile");
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        fprintf(stderr, "Ошибка при получении указателя на функцию: %s\n", dlsym_error);
        dlclose(handle);
        return 1;
    }

    int result = replaceDigitsInFile(inputFile, outputFile, replaceChar);

    dlclose(handle);

    if (result == -1) {
        printf("Произошла ошибка при выполнении программы.\n");
        return 1;
    }

    printf("Операции выполнены успешно. Количество замененных цифр: %d\n", result);
    return 0;
}
