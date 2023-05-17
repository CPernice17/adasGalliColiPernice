#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#include "../../src/header/commonFunctions.h"

FILE *steerLog;

void handleFailure() {
    fclose(steerLog);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    char steerDirection[16];
    int ecuFd;

    int readVal;

    signal(SIGUSR1, handleFailure);
    
    createLog("../../logs/actuators/steer", &steerLog);

    do {
        ecuFd = open("../../ipc/steerPipe", O_RDONLY | O_NONBLOCK);    //Opening named pipe for write
        if(ecuFd == -1){
            printf("pipe not found. Trying again...\n");
            sleep(1);
        }
    } while(ecuFd == -1);

    while (1) {
        if((readVal = readline(ecuFd, steerDirection)) == -1) {
            writeMessage(steerLog, "NO ACTION");
            sleep(1);
        }else if(strcmp(steerDirection, "SINISTRA") * strcmp(steerDirection, "DESTRA") == 0) {
            writeMessage(steerLog, "STO GIRANDO A %s", steerDirection);
            sleep(1);
        }
    }

    fclose(steerLog);
    wait(NULL);
    exit(EXIT_SUCCESS);
}