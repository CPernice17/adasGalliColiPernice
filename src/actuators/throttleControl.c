#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#include "../../src/header/commonFunctions.h"

FILE *throttleLog;

int main(int argc, char *argv[]) {
    int ecuFd;
    char str[16];
    int increment; 
    createLog("../../logs/actuators/throttle", &throttleLog);
    ecuFd = openPipeOnRead("../../ipc/throttlePipe");
    while(1) {
        readline(ecuFd, str);
        if(strncmp(str, "INCREMENTO", 10) == 0) {
            increment = atoi(str + 11);
            writeMessage(throttleLog, "AUMENTO %d", increment);
        }
    }
    fclose(throttleLog);
    wait(NULL);
    exit(EXIT_SUCCESS);
}