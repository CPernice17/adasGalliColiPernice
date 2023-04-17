#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void createLog(char *name, FILE **log) {
    char *fileName = (char*) malloc((strlen(name)+5)*sizeof(char));
    sprintf(fileName, "%s.log", name);
    remove(fileName);
    if((*log = fopen(fileName, "w")) < 0){
        exit(EXIT_FAILURE);
    }
}

void formattedTime(char *timeBuffer) {
    time_t rawTime;
    struct tm *info;
    time(&rawTime);
    info = localtime(&rawTime);
    strftime(timeBuffer,256,"%x - %X", info);
}

void writeMessage(FILE *fp, const char * format, ...){
    char buffer[256];
    char date[256];
    formattedTime(date);
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 256, format, args);
    va_end(args);
    fprintf(fp, "[%s] - [%s]\n", date, buffer);
    fflush(fp);
}

int createPipe(char *pipeName) {
    int fd;
    unlink(pipeName);
    if(mknod(pipeName, __S_IFIFO, 0) < 0 ) {    //Creating named pipe
        exit(EXIT_FAILURE);
    }
    chmod(pipeName, 0660);
    do {
        fd = open(pipeName, O_WRONLY);    //Opening named pipe for write
        if(fd == -1){
            printf("Named pipe not found. Trying again...\n");
            sleep(1);
        }
    } while(fd == -1);
    return fd;
}

int openPipeOnRead(char *pipeName) {
    int fd;
    do {
        fd = open(pipeName, O_RDONLY);    //Opening named pipe for write
        if(fd == -1){
            printf("Named pipe not found. Trying again...\n");
            sleep(1);
        }
    } while(fd == -1);
    return fd;
}

int readline (int fd, char *str) {
    int n;
    printf("Reading...\n");
    do {
        if(read (fd, str, 1) < 0) {
            printf("Read error\n");
            return -1;
        }
    } while(*str++ != '\0');
    return 0;
}