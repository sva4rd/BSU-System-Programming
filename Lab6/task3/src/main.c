#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/replacing.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Использование: %s <входной_файл> <выходной_файл> <символ>\n", argv[0]);
        return 1;
    }
    
    const char* inputFile = argv[1];
    const char* outputFile = argv[2];
    char replaceChar = argv[3][0];
    
    pid_t pid = fork();
    if (pid == -1) {
	perror("Ошибка при создании дочернего процесса");
	exit(1);
    } else if (pid == 0) {
	int result = replaceDigitsInFile(inputFile, outputFile, replaceChar);
    	if (result == -1) {
        	printf("Произошла ошибка при выполнении программы.\n");
        	exit(1);
    	}
    	printf("Операции выполнены успешно. Количество замененных цифр: %d\n", result);
	exit(0);
    }	
    
    int status;
    wait(&status);
    
    if(WIFEXITED(status)){
    	int childResult = WEXITSTATUS(status);
    	printf("Дочерний процесс завершился со статусом: %d\n", childResult);
    } else {
    	printf("Дочерний процесс завершился неудачно\n");
    }
    
    return 0;
}
