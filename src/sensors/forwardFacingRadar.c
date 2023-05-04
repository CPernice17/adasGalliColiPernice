#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/un.h> /* For AFUNIX sockets */
#include <signal.h>
#define DEFAULT_PROTOCOL 0

#include "../../src/header/commonFunctions.h"
#include "../../src/header/socketFunctions.h"

FILE *sensorLog;

void handleFailure() {
    fclose(sensorLog);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    signal(SIGUSR1, handleFailure);
    createLog("../../logs/sensors/radar", &sensorLog);
    writeMessage(sensorLog, "SENSOR LAUNCHED");
    fclose(sensorLog);
    wait(NULL);
    exit(EXIT_SUCCESS);
}