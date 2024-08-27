#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include "../include/replacing.h"

struct message {
    long type;
    char text[1024];
};

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Использование: %s <входной_файл> <выходной_файл> <символ>\n", argv[0]);
        return 1;
    }
    
    const char* inputFile = argv[1];
    const char* outputFile = argv[2];
    char replaceChar = argv[3][0];
    
    int runningProcesses = 0;
    
    key_t key = ftok("key_for_msg", 27);
    int msgid = msgget(key, 0666 | IPC_CREAT);

    if (msgid == -1) {
        perror("msgget");
        return -1;
    }
    
    pid_t pid = fork();
    if (pid == -1) {
	perror("Ошибка при создании дочернего процесса");
	exit(1);
    } else if (pid == 0) {
	int result = replaceDigitsInFile(inputFile, outputFile, replaceChar);
	struct message msg;
    	if (result == -1) {
            snprintf(msg.text, sizeof(msg.text), "Произошла ошибка при выполнении программы.");
            msg.type = 1;
            msgsnd(msgid, &msg, sizeof(msg), 0);
            exit(1);
    	}
	snprintf(msg.text, sizeof(msg.text), "Операции выполнены успешно. Количество замененных цифр: %d", result);
        msg.type = 1;
        msgsnd(msgid, &msg, sizeof(msg), 0);
	exit(0);
    } else {
        runningProcesses++;
    }
    
    while (runningProcesses > 0) {
    	struct message msg;
        msgrcv(msgid, &msg, sizeof(msg), 1, 0);
        printf("%s\n", msg.text);
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
