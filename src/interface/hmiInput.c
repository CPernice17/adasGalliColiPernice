#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../header/commonFunctions.h"

int main() {
    char cmd[32];
    int fd;
    printf("HMI Input system initialized\n\n");
    fd = createPipe("../../ipc/hmiInputToEcuPipe");
    while(1) {
        scanf("%s", &cmd);
        if(strcmp(cmd, "INIZIO") == 0) {
            printf("Intializing\n");
            write(fd, cmd, strlen(cmd)+1);
        } else if(strcmp(cmd, "PARCHEGGIO") == 0) {
            printf("Parking\n");
            write(fd, cmd, strlen(cmd)+1);
        } else if(strcmp(cmd, "ARRESTO") == 0) {
            printf("Shutting down the system\n");
            write(fd, cmd, strlen(cmd)+1);
            sleep(1);
            close(fd);
            unlink("../../ipc/hmiInputToEcuPipe.txt");
            exit(EXIT_SUCCESS);
        } else {
            printf("Command not found. Please try again\n");
        }
    }
}