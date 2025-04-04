#include <iostream>
#include "C2Server.h"
#include "TcpSocket.h"
int main(){
    try{
        auto server = std::make_unique<C2Server>(std::make_unique<TcpSocket>());
        server->run();
    } catch (const std::exception &ex){
        std::cerr <<"[-] Fatal Error: "<<ex.what()<<std::endl;
        return -1;
    }
    return 0;
}