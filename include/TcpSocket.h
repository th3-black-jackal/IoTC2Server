#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>


#include "ISocket.h"
class TcpSocket : public ISocket {
    private:
        int serverFd{};
        struct sockaddr_in address{};
        socklen_t addrlen = sizeof(address);
    
    public:
    #define PORT 4444
    #define BUF_SIZE 4096
    TcpSocket();
    ~TcpSocket();
    void bindSocket(int port) override;
    void listenToSocket() override;
    int acceptConnection() override;
    void sendToClient(int clientSocket, const std::string& data) override;
    std::string receiveFromClient(int clientSocket) override;
    void closeConnection() override;
    void closeClientConnection(int sockedFd) override;
    };

    #endif