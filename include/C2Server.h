#ifndef C2SERVER_H
#define C2SERVER_H


#include "TcpSocket.h"
#include "SQLiteConnection.h"
#include <memory>

class C2Server {
    private:
        std::unique_ptr<ISocket> socket;
        bool oneShotMode = false;
    public:
        explicit C2Server(std::unique_ptr<ISocket> s);
        void run();
        void enableOneShotMode();
};

#endif