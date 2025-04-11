#include "TcpSocket.h"
#include <stdexcept>
#include "Logger.h"

TcpSocket::TcpSocket(){
    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverFd == -1){
        throw std::runtime_error("[-] Socket creation failed");
    }
}

TcpSocket::~TcpSocket(){
    closeConnection();
}

void TcpSocket::bindSocket(int port) {
    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd == 0) throw std::runtime_error("Socket creation failed");

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(serverFd, (struct sockaddr *)&address, sizeof(address)) < 0)
        throw std::runtime_error("Bind failed");
    Logger::Instance().Log(LogLevel::INFO, "Server binded to port");

    
}

void TcpSocket::listenToSocket(){
    if (listen(serverFd, 3) < 0)
        throw std::runtime_error("Listen failed");
    Logger::Instance().Log(LogLevel::INFO, "Start listening");

}

int TcpSocket::acceptConnection() {
    int clientSocket = accept(serverFd, (struct sockaddr *)&address, &addrlen);
    if (clientSocket < 0) throw std::runtime_error("Accept failed");
    Logger::Instance().Log(LogLevel::INFO, "New connection");

    return clientSocket;
}

void TcpSocket::sendToClient(int clientSocket, const std::string& data) {
    ssize_t sent = send(clientSocket, data.c_str(), data.size(), 0);
    if(sent == -1){
        throw std::runtime_error("[-] Send failed");
    }
    
}

std::string TcpSocket::receiveFromClient(int clientSocket) {
    char buffer[BUF_SIZE] = {0};
    int bytes = recv(clientSocket, buffer, BUF_SIZE - 1, 0);
    if(bytes == -1){
        throw std::runtime_error("[-] Receive failed");
    }
    if (bytes <= 0)  {
        throw std::runtime_error("[!] Client disconnected");
    }
    return std::string(buffer, bytes);
}

void TcpSocket::closeConnection() {
    if(serverFd >= 0){
        close(serverFd);
        serverFd = -1;
    }
   
}

void TcpSocket::closeClientConnection(int socketFd){
    if(socketFd >= 0){
        close(socketFd);
        socketFd = -1;
    }
}