#pragma once
#include <string>

class IDatabaseConnection {
public:
    virtual ~IDatabaseConnection() = default;
    virtual bool connect(const std::string& dbPath) = 0;
    virtual void disconnect() = 0;
    virtual bool isConnected() const = 0;
};
