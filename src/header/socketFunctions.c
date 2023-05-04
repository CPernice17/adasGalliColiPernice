#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h> /* For AFUNIX sockets */
#define DEFAULT_PROTOCOL 0

void socketAuth(int *clientFd, struct sockaddr_un *serverAddress, int *serverLen, char *serverName) {
    *serverLen = sizeof(*serverAddress);
    *clientFd = socket(AF_UNIX, SOCK_STREAM, DEFAULT_PROTOCOL);
    struct sockaddr_un tmp = {.sun_family = AF_UNIX};
    strcpy(tmp.sun_path, serverName);
    *serverAddress = tmp;
}

void connectServer(int clientFd, struct sockaddr *serverAddressPtr, int serverLen) {
    while(connect(clientFd, serverAddressPtr, serverLen) == -1) { 
        sleep(2);
    }
}

void receiveString(int fd, char *str) {
    do{
        while(recv(fd, str, 1, 0) < 0)
            sleep(1);
    } while(*str++ != '\0');
}