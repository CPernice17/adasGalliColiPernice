#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "../header/commonFunctions.h"

void communicate(int fd, FILE *log, char *msg) {
    write(fd, msg, strlen(msg)+1);    //Writing message down pipe
    writeMessage(log, msg);
}

int main() {
    FILE *log;
    int hmiFd, hmiInputFd;
    char *msg;
    createLog("../../logs/centralECU/ecu", &log);

    hmiFd = createPipe("../../ipc/ecuToHmiPipe");
    hmiInputFd = openPipeOnRead("../../ipc/hmiInputToEcuPipe");
    communicate(hmiFd, log, "ECU initialized");

    while (1) {
        printf("ECU reading from pipe...\n");
        if(readline(hmiInputFd, msg) == 0)
            communicate(hmiFd, log, msg);
        if(strcmp(msg, "ARRESTO") == 0)
            break;
    }

    //end program
    fclose(log);
    close(hmiInputFd);
    close(hmiFd);
    unlink("../../ipc/ecuToHmiPipe");
    exit(EXIT_SUCCESS);
}