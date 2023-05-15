#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

#include "../../src/header/commonFunctions.h"

FILE *throttleLog;

int main(int argc, char *argv[]) {
    int ecuFd;
    char str[16];
    char randName[128];
    FILE *rnd;
    int increment;
    int randomNum;
    
    if(strcmp(argv[1], "NORMALE") == 0) {
        sprintf(randName, "/dev/random");
    } else if(strcmp(argv[1], "ARTIFICIALE") == 0) {
        sprintf(randName, "../../build/randomARTIFICIALE.binary");
    }

    rnd = fopen(randName, "r");

    createLog("../../logs/actuators/throttle", &throttleLog);
    ecuFd = openPipeOnRead("../../ipc/throttlePipe");
    while(1) {
        readline(ecuFd, str);
        if(strncmp(str, "INCREMENTO", 10) == 0) {
            increment = atoi(str + 11);
            writeMessage(throttleLog, "AUMENTO %d", increment);
            while(fread(&randomNum, sizeof(int), 1, rnd) < 0);
            randomNum = randomNum % 100000;
            if(randomNum == 0) {
                kill(getppid(), SIGUSR1);
                exit(EXIT_FAILURE);
            }
        }
    }
    fclose(throttleLog);
    exit(EXIT_SUCCESS);
}