#ifndef INCLUDED_SERVER_MANAGER
#define INCLUDED_SERVER_MANAGER

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#define DEFAULT_PORT (unsigned short) 2019

class ServerManager
{
    public:
    ServerManager(unsigned short inPort = DEFAULT_PORT);
    ~ServerManager();
    unsigned short getPort();
    bool startUp();
    bool waitForClient();
    private:
    bool started;
    unsigned short port;
    WSADATA wsaData;
    sockaddr_in serverParam;
    sockaddr_in clientParam;
    int clientParamLen;
    SOCKET serverSocket;
    SOCKET clientSocket;
};

#endif