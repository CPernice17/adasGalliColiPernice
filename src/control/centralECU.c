#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h> /* For AFUNIX sockets */

#include "../../src/header/commonFunctions.h"
#include "../../src/header/socketFunctions.h"

#define DEFAULT_PROTOCOL 0
#define READ 0
#define WRITE 1

void communicate(int fd, FILE *log, char *msg) {
    write(fd, msg, strlen(msg)+1);    //Writing message down pipe
    writeMessage(log, msg);
}

int getInput(int hmiInputFd, int hmiFd, FILE *log) {
    char str[32];
    if(readline(hmiInputFd, str) == 0) {
        if(strcmp(str, "ARRESTO") == 0)
            return 1;
        else if(strcmp(str, "INIZIO") == 0)
            return 2;
        else if(strcmp(str, "PARCHEGGIO") == 0)
            return 3;
        return -1;
    }
}

void killProcesses(int pids[7]) {
    for(int i = 0; i < 7; i++) {
        kill(pids[i], SIGTERM);
    }
}

int main(int argc, char *argv[]) {
    FILE *log;
    int hmiFd, hmiInputFd;
    int anonFd[2];
    char *msg;
    createLog("../../logs/centralECU/ecu", &log);

    int inputReader;    // Input reader PID
    int input = 0;  // Input from HMIInput

    int ecuFd, clientFd, ecuLen, clientLen;

    struct sockaddr_un ecuUNIXAddress; /*Server address */
    struct sockaddr *ecuSockAddrPtr;   /*Ptr to server address*/
    struct sockaddr_un clientUNIXAddress; /*Client address */
    struct sockaddr *clientSockAddrPtr;   /*Ptr to client address*/

    int isListening[3] = {0,0,0};
    int sensor;  // Indicates which sensor wants to send data

    hmiFd = createPipe("../../ipc/ecuToHmiPipe");
    hmiInputFd = openPipeOnRead("../../ipc/hmiInputToEcuPipe");

    int pid[7];
    char *components[] = {"../actuators/steerByWire","../actuators/throttleControl"
        ,"../actuators/brakeByWire","../sensors/frontWindshieldCamera"
        ,"../sensors/forwardFacingRadar","../sensors/parkAssist"
        ,"../sensors/surroundViewCameras"};

    for(int i = 0; i < 7; i++) {
        if((pid[i] = fork()) < 0) {
            exit(EXIT_FAILURE);
        } else if(pid[i] == 0) {
            printf("Executing %s\n", components[i]);
            execl(components[i], 0);
            exit(EXIT_SUCCESS);
        }
    }

    ecuSockAddrPtr = (struct sockaddr *) &ecuUNIXAddress;
    ecuLen = sizeof(ecuUNIXAddress);
    clientSockAddrPtr = (struct sockaddr *) &clientUNIXAddress;
    clientLen = sizeof(clientUNIXAddress);

    ecuFd = socket(AF_UNIX, SOCK_STREAM, DEFAULT_PROTOCOL);
    ecuUNIXAddress.sun_family = AF_UNIX;    /* Set domain type */

    strcpy(ecuUNIXAddress.sun_path, "../../ipc/ecuSocket");
    unlink("../../ipc/ecuSocket");

    bind(ecuFd, ecuSockAddrPtr, ecuLen);
    listen(ecuFd, 3);

    pipe2(anonFd, O_NONBLOCK);

    if((inputReader = fork()) < 0) {
        exit(EXIT_FAILURE);
    } else if(inputReader == 0) {   //Reads from HMI Input terminal
        close(anonFd[READ]);
        while (1) {
            input = getInput(hmiInputFd, hmiFd, log);
            write(anonFd[WRITE], &input, sizeof(int));
            if(input == 1) {
                write(hmiFd, "ARRESTO", strlen("ARRESTO")+1);
                close(anonFd[WRITE]);
                exit(EXIT_SUCCESS);
            }
        }
    }
    close(anonFd[WRITE]);
    while(1) {
        read(anonFd[READ], &input, sizeof(int));
        if(input == 1)
            break;
        else if(input == 2) {
            isListening[0] = 1;
            isListening[1] = 1;
            isListening[2] = 0;
        }
        else if(input == 3){
            isListening[0] = 0;
            isListening[1] = 0;
            isListening[2] = 1;
        }
        printf("Input: %d\n", input);
        clientFd = accept(ecuFd, clientSockAddrPtr, &clientLen);
        if(fork() == 0){ /*Create child to handle requests*/
            while(recv(clientFd, &sensor, sizeof(int), 0) < 0);
            while(send(clientFd, &isListening[sensor], sizeof(int), 0) < 0);
            if(isListening[sensor] == 0)
                exit(EXIT_SUCCESS);
            char str[16];
            printf("%s\n", str);
            exit(EXIT_SUCCESS);
        }
        close(clientFd);
    }

    killProcesses(pid);
    close(anonFd[READ]);
    fclose(log);
    close(hmiInputFd);
    close(hmiFd);
    unlink("../../ipc/ecuSocket");
    unlink("../../ipc/ecuToHmiPipe");
    exit(EXIT_SUCCESS);
}