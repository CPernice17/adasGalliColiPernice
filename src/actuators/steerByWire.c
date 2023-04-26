#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "../../src/header/commonFunctions.h"

int main(int argc, char *argv[]) {
    FILE *log;
    char steerDirection[16];
    int ecuFd;

    int readVal;
    
    createLog("../../logs/actuators/steer", &log);

    do {
        ecuFd = open("../../ipc/steerPipe", O_RDONLY | O_NONBLOCK);    //Opening named pipe for write
        if(ecuFd == -1){
            printf("pipe not found. Trying again...\n");
            sleep(1);
        }
    } while(ecuFd == -1);

    while (1) {
        if((readVal = readline(ecuFd, steerDirection)) == -1) {
            writeMessage(log, "NO ACTION");
        }else if(strcmp(steerDirection, "SINISTRA") * strcmp(steerDirection, "DESTRA") == 0) {
            for(int i = 0; i < 4 || strcmp(steerDirection, "PARCHEGGIO") == 0; i++) {
                readline(ecuFd, steerDirection);
                writeMessage(log, "STO GIRANDO A %s", steerDirection);
                sleep(1);
            }
        }
        sleep(1);
    }

    fclose(log);
    wait(NULL);
    exit(EXIT_SUCCESS);
}