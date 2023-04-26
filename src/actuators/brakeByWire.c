#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#include "../../src/header/commonFunctions.h"

FILE *steerLog;

void handleStop(int signal) {
    writeMessage(steerLog, "ARRESTO AUTO");
}

void handleTerm(int signal) {
    fclose(steerLog);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    signal(SIGTSTP, handleStop);
    int ecuFd;
    char str[16];
    int decrement; 
    createLog("../../logs/actuators/brake", &steerLog);
    ecuFd = openPipeOnRead("../../ipc/brakePipe");
    while(1) {
        readline(ecuFd, str);
        if(strncmp(str, "FRENO", 5) == 0) {
            decrement = atoi(str + 6);
            writeMessage(steerLog, "FRENO %d", decrement);
        }
    }
}