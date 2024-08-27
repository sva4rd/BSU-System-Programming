#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "../include/replacing.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Использование: %s <входной_файл> <выходной_файл> <символ>\n", argv[0]);
        return 1;
    }
    
    const char* inputFile = argv[1];
    const char* outputFile = argv[2];
    char replaceChar = argv[3][0];
    
    int runningProcesses = 0;

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return -1;
    }
    
    pid_t pid = fork();
    if (pid == -1) {
	perror("Ошибка при создании дочернего процесса");
	exit(1);
    } else if (pid == 0) {
    	close(pipefd[0]);
	int result = replaceDigitsInFile(inputFile, outputFile, replaceChar);
	char resultStr[1024];
    	if (result == -1) {
	    snprintf(resultStr, sizeof(resultStr), "Произошла ошибка при выполнении программы.");
	    write(pipefd[1], resultStr, strlen(resultStr));
            exit(1);
    	}
	snprintf(resultStr, sizeof(resultStr), "Операции выполнены успешно. Количество замененных цифр: %d", result);
	write(pipefd[1], resultStr, strlen(resultStr));
	exit(0);
    } else {
        runningProcesses++;
    }
    
    while (runningProcesses > 0) {
        close(pipefd[1]);
        char buffer[1024];
        ssize_t bytesRead = read(pipefd[0], buffer, 1024);
        buffer[bytesRead] = '\0';
        printf("%s\n", buffer);
        int status;
        pid_t finished = wait(&status);
        if(WIFEXITED(status)){
    	   int childResult = WEXITSTATUS(status);
    	   printf("Дочерний процесс завершился со статусом: %d\n", childResult);
    	} else {
    	   printf("Дочерний процесс завершился неудачно\n");
    	}
    	runningProcesses--;
    }
    return 0;
}
