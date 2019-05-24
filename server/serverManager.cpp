#include "serverManager.h"
#include <string>
#include <chrono>

ServerManager::ServerManager(unsigned short inPort)
{
    port = inPort;
    started = false;
    hasClient = false;
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
        std::cout << "WSA init failure!" << std::endl << WSAGetLastError() << std::endl;
        return false;
    }
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //TCP socket, IPv4
    int size = 0x800000;
    setsockopt(serverSocket, SOL_SOCKET, SO_SNDBUF, (char*)&size, sizeof(size));
    setsockopt(serverSocket, SOL_SOCKET, SO_RCVBUF, (char*)&size, sizeof(size));
    if(serverSocket == INVALID_SOCKET)
    {
        std::cout << "Socket init failure!" << std::endl << WSAGetLastError() << std::endl;
        return false;
    }
    if(bind(serverSocket, (sockaddr*) &serverParam, sizeof(serverParam)) != 0)
    {
        std::cout << "Bind failure!" << std::endl << WSAGetLastError() << std::endl;
        return false;
    }
    if(listen(serverSocket, SOMAXCONN) != 0)
    {
        std::cout << "Listening failure!" << std::endl << WSAGetLastError() << std::endl;
        return false;
    }
    started = true;
    return true;
}

bool ServerManager::waitForClient()
{
    if(!started)
    {
        std::cout << "Error: tried to wait for a client before starting the server!" << std::endl;
        return false;
    }
    clientSocket = accept(serverSocket, (sockaddr*) &clientParam, &clientParamLen);
    if(clientSocket == INVALID_SOCKET)
    {
        std::cout << "Client connection failure!" << std::endl << WSAGetLastError() << std::endl;
        return false;
    }
    int yes = 1;
    int size = 0x800000;
    setsockopt(clientSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&yes, sizeof(yes));
    setsockopt(clientSocket, SOL_SOCKET, SO_SNDBUF, (char*)&size, sizeof(size));
    setsockopt(clientSocket, SOL_SOCKET, SO_RCVBUF, (char*)&size, sizeof(size));
    hasClient = true;
    return true;
}

void ServerManager::sendBuffer(float* buffer)
{
    if(!hasClient)
    {
        std::cout << "Cannot send buffer to client; client not found!" << std::endl;
        return;
    }
    char array[8*SAMPLE_COUNT_PER_BUFFER];
    memcpy(array, buffer, 8*SAMPLE_COUNT_PER_BUFFER);
    char temp;
    //auto t1 = std::chrono::high_resolution_clock::now();
    /*for(int i = 0; i < 2*SAMPLE_COUNT_PER_BUFFER; ++i)
    {
        temp = array[4*i];
        array[4*i] = array[4*i+3];
        array[4*i+3] = temp;
        temp = array[4*i+1];
        array[4*i+1] = array[4*i+2];
        array[4*i+2] = temp;
    }*/
    /*auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> d6 = t2-t1;
    std::cout << "Loop done in " + std::to_string(d6.count()) << std::endl;*/
    if(send(clientSocket, array, 8*SAMPLE_COUNT_PER_BUFFER, 0) != 8*SAMPLE_COUNT_PER_BUFFER)
    {
        std::cout << "Cannot send buffer to client!" << std::endl;
    }
}