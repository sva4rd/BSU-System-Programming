#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <pthread.h>
#include "../include/file_search.h"

typedef struct {
    const char *dirPath;
    const char *searchString;
    const char *resultFile;
    const char *fileName;
} thread_data_t;

void *thread_function(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    char filePath[1024];

    if (snprintf(filePath, sizeof(filePath), "%s/%s", data->dirPath, data->fileName) >= sizeof(filePath)) {
        fprintf(stderr, "Error: long path\n");
        free(data);
        return NULL;
    }

    searchInFile(filePath, data->searchString, data->resultFile);
    free(data);
    return NULL;
}

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
    pthread_t threads[N];
    int activeThreads = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            if (activeThreads >= N) {
                pthread_join(threads[activeThreads % N], NULL); 
            }

            thread_data_t *data = malloc(sizeof(thread_data_t));
            if (data == NULL) {
                fprintf(stderr, "Ошибка выделения памяти.\n");
                continue;
            }

            data->dirPath = dirPath;
            data->searchString = searchString;
            data->resultFile = resultFile;
            data->fileName = entry->d_name;

            if (pthread_create(&threads[activeThreads % N], NULL, thread_function, data) != 0) {
                perror("Thread creation failed");
                free(data);
                continue;
            }

            activeThreads++;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < N; ++i) {
        if (i < activeThreads) {
            pthread_join(threads[i], NULL);
        }
    }

    closedir(dir);
    return 0;
}

