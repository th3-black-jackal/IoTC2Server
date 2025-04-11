#include "SQLiteConnection.h"
#include <iostream>

SQLiteConnection::SQLiteConnection() : db(nullptr), connected(false) {}

SQLiteConnection::~SQLiteConnection() {
    disconnect();
}

bool SQLiteConnection::connect(const std::string& dbPath) {
    int rc = sqlite3_open_v2(dbPath.c_str(), &db, SQLITE_OPEN_READWRITE, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "[SQLite Error] " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    connected = true;
    return true;
}

void SQLiteConnection::disconnect() {
    if (connected && db) {
        sqlite3_close(db);
        connected = false;
    }
}

bool SQLiteConnection::isConnected() const {
    return connected;
}

sqlite3* SQLiteConnection::getRaw() {
    return db;
}
