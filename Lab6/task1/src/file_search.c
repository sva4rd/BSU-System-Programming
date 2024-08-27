#include "../include/file_search.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void searchInFile(const char *filePath, const char *searchString, const char *resultFile) {
    FILE *file = fopen(filePath, "r");
    if (!file) {
        perror("File opening failed");
        return;
    }

    int len = strlen(searchString);
    char buffer[len];
    size_t bytesRead;
    char* found = "no";

    while ((bytesRead = fread(buffer, 1, len, file)) == len) {
        if (memcmp(buffer, searchString, len) == 0) {
            found = "yes";
            break;
        }
        fseek(file, 1 - len, SEEK_CUR);
    }

    FILE *resFile = fopen(resultFile, "a");
    if (!resFile) {
        perror("Result File opening failed");
        return;
    }
    fprintf(resFile, "PID: %d, File: %s, Bytes: %ld, Found: %s\n", getpid(), filePath, ftell(file), found);
    fclose(resFile);

    fclose(file);
}
