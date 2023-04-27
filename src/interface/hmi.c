#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> 
#include <string.h>

#include "../../src/header/commonFunctions.h"

int readFromPipe(int fd) {
    char str[256];
    if(readline(fd, str) == 0) {
        if(strcmp(str, "PARCHEGGIO") == 0)
            return -1;
        printf("%s\n", str);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    int pidInput, pidECU;
    if((pidInput = fork()) < 0) {
        exit(EXIT_FAILURE);
    } else if(pidInput == 0) {
        printf("Executing HMIInput\n");
        execlp("/bin/gnome-terminal", "gnome-terminal", "-e", "./hmiInput", 0);
    } 
    if((pidECU = fork()) < 0) {
        exit(EXIT_FAILURE);
    }
    else if(pidECU == 0) {
        printf("Executing centralECU\n");
        execlp("../control/centralECU", 0);
    }

    printf("HMI Output system initialized\n\n");
    int fd;
    int n;
    fd = openPipeOnRead("../../ipc/ecuToHmiPipe");
    printf("Named pipe found.\n\n");
    while(1) {
        if(readFromPipe(fd) < 0)
            break;
    }
    for(int i = 0; i < 2; i++) {
        wait(NULL);
    }
    close(fd);
    exit(EXIT_SUCCESS);
}