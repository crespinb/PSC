#ifndef INCLUDED_SERVER_MANAGER
#define INCLUDED_SERVER_MANAGER

#include "config.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

class ServerManager
{
    public:
    ServerManager(unsigned short inPort = SERVER_PORT);
    ~ServerManager();
    unsigned short getPort();
    bool startUp();
    bool waitForClient();
    void sendBuffer(float* buffer);
    private:
    bool started;
    bool hasClient;
    unsigned short port;
    WSADATA wsaData;
    sockaddr_in serverParam;
    sockaddr_in clientParam;
    int clientParamLen;
    SOCKET serverSocket;
    SOCKET clientSocket;
};

#endif