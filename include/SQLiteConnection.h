#pragma once
#include <sqlite3.h>
#include <string>

class SQLiteConnection {
public:
    SQLiteConnection();
    ~SQLiteConnection();

    bool connect(const std::string& dbPath);
    void disconnect();
    bool isConnected() const;
    sqlite3* getRaw();

private:
    sqlite3* db;
    bool connected;
};
