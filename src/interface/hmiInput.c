#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include "../../src/header/commonFunctions.h"

int fd;

void signalHandler() {
    close(fd);
    unlink("../../ipc/hmiInputToEcuPipe");
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    char cmd[32];
    int pid = getpid();

    signal(SIGINT, signalHandler);

    printf("HMI Input system initialized\n\n");
    fd = createPipe("../../ipc/hmiInputToEcuPipe");
    write(fd, &pid, sizeof(int));
    while(1) {
        scanf("%s", &cmd);
        if(strcmp(cmd, "INIZIO") == 0) {
            write(fd, cmd, strlen(cmd)+1);
        } else if(strcmp(cmd, "PARCHEGGIO") == 0) {
            write(fd, cmd, strlen(cmd)+1);
        } else if(strcmp(cmd, "ARRESTO") == 0) {
            write(fd, cmd, strlen(cmd)+1);
        } else {
            printf("Command not found. Please try again\n");
        }
    }
}