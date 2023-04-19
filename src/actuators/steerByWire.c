#include <stdio.h>
#include <stdlib.h>

#include "../../src/header/commonFunctions.h"

int main(int argc, char *argv[]) {
    FILE *log;
    createLog("../../logs/actuators/steer", &log);
    writeMessage(log, "ACTUATOR LAUNCHED");
    fclose(log);
    exit(EXIT_SUCCESS);
}