#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h> 
#include <string.h>

#include "../../src/header/commonFunctions.h"

int pidECU;
int terminated;
int fd;

void countTerminated() {
    terminated++;
    if (terminated >= 2) {
        close(fd);
        exit(EXIT_SUCCESS);
    }
}

int readFromPipe(int fd) {
    char str[256];
    if(readline(fd, str) < 0) {
        return -1;
    }
    printf("%s\n", str);
    return 0;
}

void failureHandler() {
    printf("ERRORE ACCELERAZIONE\nTERMINAZIONE PROGRAMMA...");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    signal(SIGCHLD, countTerminated);
    signal(SIGUSR1, failureHandler);
    terminated = 0;

    if(argc != 2 || strcmp(argv[1], "NORMALE") * strcmp(argv[1], "ARTIFICIALE") != 0) {
        printf("Argomenti non validi. Riprovare\n");
        exit(EXIT_FAILURE);
    }

    system("gnome-terminal -- ./hmiInput");
    if((pidECU = fork()) < 0) {
        exit(EXIT_FAILURE);
    }
    else if(pidECU == 0) {
        execl("../control/centralECU", "./centralECU", argv[1], 0);
    }

    printf("HMI Output system initialized\n\n");
    int n;
    fd = openPipeOnRead("../../ipc/ecuToHmiPipe");
    printf("Named pipe found.\n\n");
    while(1) {
        while(readFromPipe(fd) < 0);
    }
}