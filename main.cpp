#include <iostream>
#include <signal.h>
#include "C2Server.h"
#include "TcpSocket.h"
#include "Logger.h"

/*
A temporary solution for SIGPIE signal
*/
void sigpie_handler(int s){
    Logger::Instance().Log(LogLevel::WARN, "SIGPIE triggered - Current connection may not work properly");

}
int main(){
    signal(SIGPIPE, sigpie_handler);
    try{
        auto server = std::make_unique<C2Server>(std::make_unique<TcpSocket>());
        server->run();
    } catch (const std::exception &ex){
        std::cerr <<"[-] Fatal Error: "<<ex.what()<<std::endl;
        return -1;
    }
    return 0;
}