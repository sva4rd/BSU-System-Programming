#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include "../include/file_search.h"

int main(int argc, char **argv) {
    if (argc != 5) {
        printf("Usage: %s <directory> <search_string> <result_file> <N>\n", argv[0]);
        return 1;
    }

    const char *dirPath = argv[1];
    const char *searchString = argv[2];
    const char *resultFile = argv[3];
    int N = atoi(argv[4]);
    
    if (N <= 0) {
    	fprintf(stderr, "Error: wrong <N>\n");
    	return 1;
    }

    DIR *dir = opendir(dirPath);
    if (!dir) {
        perror("Directory opening failed");
        return 1;
    }
    
    FILE *resFile = fopen(resultFile, "w");
    if (!resFile) {
        perror("Result File opening failed");
        return 1;
    }
    fclose(resFile);

    struct dirent *entry;
    int activeProcesses = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            if (activeProcesses >= N) {
                wait(NULL);
                activeProcesses--;
            }

            pid_t pid = fork();
            if (pid == 0) {
                char filePath[1024];
                snprintf(filePath, sizeof(filePath), "%s/%s", dirPath, entry->d_name);
                searchInFile(filePath, searchString, resultFile);
                exit(0);
            } else if (pid > 0) {
                activeProcesses++;
            } else {
                perror("Fork failed");
                return 1;
            }
        }
    }

    while (activeProcesses > 0) {
        wait(NULL);
        activeProcesses--;
    }

    closedir(dir);
    return 0;
}

