#include "serverManager.h"

ServerManager::ServerManager(unsigned short inPort)
{
    port = inPort;
    started = false;
    serverParam.sin_addr.s_addr = INADDR_ANY; //any in_addr is valid
    serverParam.sin_port = htons(port);
    serverParam.sin_family = AF_INET; //TCP
    clientParam = {0};
    clientParamLen = sizeof(serverParam);
}

ServerManager::~ServerManager()
{
    closesocket(serverSocket);
    WSACleanup();
}

unsigned short ServerManager::getPort()
{
    return port;
}

bool ServerManager::startUp()
{
    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
    {
        std::cout << "WSA init failure" << std::endl << WSAGetLastError() << std::endl;
        return false;
    }
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //TCP socket, IPv4
    if(serverSocket == INVALID_SOCKET)
    {
        std::cout << "socket init failure" << std::endl << WSAGetLastError() << std::endl;
        return false;
    }
    if(bind(serverSocket, (sockaddr*) &serverParam, sizeof(serverParam)) != 0)
    {
        std::cout << "bind failure" << std::endl << WSAGetLastError() << std::endl;
        return false;
    }
    if(listen(serverSocket, SOMAXCONN) != 0)
    {
        std::cout << "listen startup failure" << std::endl << WSAGetLastError() << std::endl;
        return false;
    }
    started = true;
    return true;
}

bool ServerManager::waitForClient()
{
    if(!started)
    {
        std::cout << "tried to get client before server startup" << std::endl;
        return false;
    }
    clientSocket = accept(serverSocket, (sockaddr*) &clientParam, &clientParamLen);
    if(clientSocket == INVALID_SOCKET)
    {
        std::cout << "client connection failure" << std::endl << WSAGetLastError() << std::endl;
        return false;
    }
    return true;
}