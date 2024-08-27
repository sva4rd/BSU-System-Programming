#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <pthread.h>
#include "../include/replacing.h"

struct ThreadArgs{
    char* inputFile;
    char* outputFile;
    char replaceChar;
    int result;
};

void *thread_function(void *arg){
    struct ThreadArgs *args = (struct ThreadArgs *)arg;
    args->result = replaceDigitsInFile(args->inputFile, args->outputFile, args->replaceChar);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Использование: %s <входной_файл> <выходной_файл> <символ>\n", argv[0]);
        return 1;
    }
    
    pthread_t thread;
    struct ThreadArgs args;
    
    args.inputFile = argv[1];
    args.outputFile = argv[2];
    args.replaceChar = argv[3][0];
    
    if (pthread_create(&thread, NULL, thread_function, &args)) {
    	printf("Ошибка при создании потока");
    	return 1;
    }
    
    pthread_join(thread, NULL);
    if (args.result == -1){
    	printf("Произошла ошибка при выполнении программы.\n");
    	return 1; 	
    } else {
    	printf("Операции выполнены успешно. Количество замененных цифр: %d\n", args.result);
    }
        
    return 0;
}
