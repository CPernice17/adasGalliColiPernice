void createLog(char *name, FILE **log);

void formattedTime(char *timeBuffer);

void writeMessage(FILE *fp, const char * format, ...);

int createPipe(char *pipeName);

int openPipeOnRead(char *pipeName);

int readline (int fd, char *str);

void writeMessageToPipe(int pipeFd, const char * format, ...);