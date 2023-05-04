#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/un.h> /* For AFUNIX sockets */
#include <string.h>
#include <unistd.h>
#define DEFAULT_PROTOCOL 0

#include "../../src/header/commonFunctions.h"
#include "../../src/header/socketFunctions.h"

int main(int argc, char *argv[]) {
    FILE *sensorLog;
    FILE *urand;
    char urandName[128];
    if(strcmp(argv[1], "NORMALE") == 0) {
        sprintf(urandName, "/dev/urandom");
    } else if(strcmp(argv[1], "ARTIFICIALE") == 0) {
        sprintf(urandName, "../../build/urandomARTIFICIALE.binary");
    }
    char str[8];
    if((sensorLog = fopen("../../logs/sensors/assist.log", "a")) < 0){
        exit(EXIT_FAILURE);
    }
    urand = fopen(urandName, "rb");
    
    const int sensorID = 2; // Value in order to be recognized by the socket
    int isListening;    // Indicates whether ECU is listening or not

    int ecuFd, ecuLen;
    struct sockaddr_un ecuUNIXAddress;
    struct sockaddr *ecuSockAddrPtr = (struct sockaddr*) &ecuUNIXAddress;

    socketAuth(&ecuFd, &ecuUNIXAddress, &ecuLen, "../../ipc/ecuSocket");
    connectServer(ecuFd, ecuSockAddrPtr, ecuLen);


    while(send(ecuFd, &sensorID, sizeof(sensorID), 0) < 0);
    while(recv(ecuFd, &isListening, sizeof(sensorID), 0) < 0);

    int count = 0;

    while(count < 30 && isListening == 1) {
        for(int i = 0; i < 4; i++) {
            int c1 = fgetc(urand);
            int c2 = fgetc(urand);
            sprintf(str, "0x%02x%02x", c1, c2);
            while(send(ecuFd, str, strlen(str)+1, 0) < 0);
            writeMessage(sensorLog, "%s", str);
        }
        count++;
        sleep(1);
    }

    fclose(sensorLog);
    exit(EXIT_SUCCESS);
}