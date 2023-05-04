#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/un.h> /* For AFUNIX sockets */
#include <ctype.h>
#define DEFAULT_PROTOCOL 0

int isNumber(char *str) {
    for(int i = 0; str[i] != '\0'; i++) {
        if(!isdigit(str[i]))
            return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    FILE *rbin = fopen("../../build/urandomARTIFICIALE.binary", "rb");
    char str[8];
    for(int i = 0; i < 4; i++) {
        int c1 = fgetc(rbin);
        int c2 = fgetc(rbin);
        sprintf(str, "0x%02x%02x", c1, c2);
        printf("%s, %d\n", str,isNumber(str));
    }

    fclose(rbin);

    return 0;
}