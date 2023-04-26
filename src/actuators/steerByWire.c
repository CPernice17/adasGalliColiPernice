#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

#include "../../src/header/commonFunctions.h"

int main(int argc, char *argv[]) {
    FILE *log;
    char steerDirection[256];
    
    createLog("../../logs/actuators/steer", &log);
    writeMessage(log, "ACTUATOR LAUNCHED");
    
    int steerPipe = openPipeOnRead("../../ipc/steerPipe");

    while (1) {
        readline(steerPipe, steerDirection);
        if(strcmp(steerDirection, "SINISTRA") * strcmp(steerDirection, "DESTRA") == 0) {
            writeMessage(log, "STO GIRANDO A %s", steerDirection);
        } else {
            writeMessage("NO ACTION");
        }
        sleep(1);
    }

    fclose(log);
    wait(NULL);
    exit(EXIT_SUCCESS);
}