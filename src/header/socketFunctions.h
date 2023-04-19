void socketAuth(int *clientFd, struct sockaddr_un *serverAddress, int *serverLen, char *serverName);

void connectServer(int clientFd, struct sockaddr *serverAddressPtr, int serverLen);

void receiveString(int fd, char *str);