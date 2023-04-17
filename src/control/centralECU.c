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

int sendMessageToHMI(int hmiInputFd, int hmiFd, FILE *log) {
    char str[32];
    if(readline(hmiInputFd, str) == 0) {
        communicate(hmiFd, log, str);
        if(strcmp(str, "ARRESTO") == 0)
            return -1;
        return 0;
    }
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
        if(sendMessageToHMI(hmiInputFd, hmiFd, log) < 0)
            break;
        sleep(1);
    }

    //end program
    fclose(log);
    close(hmiInputFd);
    close(hmiFd);
    unlink("../../ipc/ecuToHmiPipe");
    exit(EXIT_SUCCESS);
}