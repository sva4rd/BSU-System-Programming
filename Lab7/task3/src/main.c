#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <string.h>
#include "../include/replacing.h"

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void semwait(int semid) {
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = -1;
    buf.sem_flg = 0;
    if (semop(semid, &buf, 1) == -1) {
        perror("semop");
        exit(1);
    }
}

void semsignal(int semid) {
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = 1;
    buf.sem_flg = 0;
    if (semop(semid, &buf, 1) == -1) {
        perror("semop");
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Использование: %s <входной_файл> <выходной_файл> <символ>\n", argv[0]);
        return 1;
    }
    
    const char* inputFile = argv[1];
    const char* outputFile = argv[2];
    char replaceChar = argv[3][0];
    
    int runningProcesses = 0;
    
    int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        return -1;
    }

    int *results_shm = (int *)shmat(shmid, NULL, 0);

    int semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget");
        return -1;
    }

    union semun arg;
    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) == -1) {
        perror("semctl");
        return -1;
    }
    
    pid_t pid = fork();
    if (pid == -1) {
	perror("Ошибка при создании дочернего процесса");
	exit(1);
    } else if (pid == 0) {
    	semwait(semid);
        results_shm[0] = replaceDigitsInFile(inputFile, outputFile, replaceChar);
        semsignal(semid);
        if (results_shm[0] == -1) {
            exit(1);
    	} else {
    	    exit(0);
    	}
    } else {
        runningProcesses++;
    }
    
    while (runningProcesses > 0) {
        int status;
        pid_t finished = wait(&status);
        if (results_shm[0] == -1) {
    	    printf("Произошла ошибка при выполнении программы\n");
    	} else {
    	    printf("Операции выполнены успешно. Количество замененных цифр: %d\n", results_shm[0]);
    	}
    	
        if(WIFEXITED(status)){
    	   int childResult = WEXITSTATUS(status);
    	   printf("Дочерний процесс завершился со статусом: %d\n", childResult);
    	} else {
    	   printf("Дочерний процесс завершился неудачно\n");
    	}
    	runningProcesses--;
    }
    
    shmdt(results_shm);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID, arg);
    return 0;
}
