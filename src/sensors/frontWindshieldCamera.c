#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h> /* For AFUNIX sockets */
#define DEFAULT_PROTOCOL 0

#include "../../src/header/commonFunctions.h"
#include "../../src/header/socketFunctions.h"

int main(int argc, char *argv[]) {
    FILE *log;
    createLog("../../logs/sensors/camera", &log);
    
    int ecuFd, ecuLen;
    struct sockaddr_un registerUNIXAddress;
    struct sockaddr *registerSockAddrPtr = (struct sockaddr*) &registerUNIXAddress;

    fclose(log);
    exit(EXIT_SUCCESS);
}