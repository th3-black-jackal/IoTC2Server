#ifndef C2SERVER_H
#define C2SERVER_H


#include "TcpSocket.h"
#include <memory>

class C2Server {
    private:
        std::unique_ptr<ISocket> socket;
    
    public:
        explicit C2Server(std::unique_ptr<ISocket> s);
        void run();
};

#endif