#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

#include "../../src/header/commonFunctions.h"

FILE *brakeLog;

void handleStop() {
    writeMessage(brakeLog, "ARRESTO AUTO");
}

void handleFailure() {
    fclose(brakeLog);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    signal(SIGTSTP, handleStop);
    signal(SIGUSR1, handleFailure);
    int ecuFd;
    char str[16];
    int decrement; 
    createLog("../../logs/actuators/brake", &brakeLog);
    ecuFd = openPipeOnRead("../../ipc/brakePipe");
    while(1) {
        readline(ecuFd, str);
        if(strncmp(str, "FRENO", 5) == 0) {
            decrement = atoi(str + 6);
            writeMessage(brakeLog, "FRENO %d", decrement);
        }
    }
}