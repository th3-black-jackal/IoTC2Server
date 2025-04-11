#ifndef ISOCKET_H
#define ISOCKET_H

#include <string>
class ISocket {
    public:
        virtual ~ISocket() = default;
        virtual void bindSocket(int port) = 0;
        virtual void listenToSocket() = 0;
        virtual int acceptConnection() = 0;
        virtual void sendToClient(int clientSocket, const std::string& data) = 0;
        virtual std::string receiveFromClient(int clientSocket) = 0;
        virtual void closeConnection() = 0;
        virtual void closeClientConnection(int socketFd) = 0;
    };

#endif