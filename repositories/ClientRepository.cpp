#include "ClientRepository.h"
#include <sqlite3.h>
#include <iostream>

ClientRepository::ClientRepository(SQLiteConnection* conn) : connection(conn) {}

bool ClientRepository::create(const Client& client) {
    const char* sql = R"(
        INSERT INTO clients (client_name, device_type, client_ip, last_seen, metadata)
        VALUES (?, ?, ?, ?, ?);
    )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(connection->getRaw(), sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, client.getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, client.getDeviceType().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, client.getIp().c_str(), -1, SQLITE_TRANSIENT);
    
    if (client.getLastSeen())
        sqlite3_bind_text(stmt, 4, client.getLastSeen()->c_str(), -1, SQLITE_TRANSIENT);
    else
        sqlite3_bind_null(stmt, 4);

    if (client.getMetadata())
        sqlite3_bind_text(stmt, 5, client.getMetadata()->c_str(), -1, SQLITE_TRANSIENT);
    else
        sqlite3_bind_null(stmt, 5);

    bool success = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return success;
}

std::optional<Client> ClientRepository::getById(int id) {
    const char* sql = R"(
        SELECT client_id, client_name, device_type, client_ip, registration_timestamp, last_seen, metadata
        FROM clients WHERE client_id = ?;
    )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(connection->getRaw(), sql, -1, &stmt, nullptr) != SQLITE_OK)
        return std::nullopt;

    sqlite3_bind_int(stmt, 1, id);

    Client client;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        client.setId(sqlite3_column_int(stmt, 0));
        client.setName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        client.setDeviceType(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        client.setIp(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        client.setRegistrationTime(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));

        const char* lastSeen = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        client.setLastSeen(lastSeen ? std::optional<std::string>{lastSeen} : std::nullopt);

        const char* metadata = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        client.setMetadata(metadata ? std::optional<std::string>{metadata} : std::nullopt);
    } else {
        sqlite3_finalize(stmt);
        return std::nullopt;
    }

    sqlite3_finalize(stmt);
    return client;
}

std::vector<Client> ClientRepository::getAll() {
    const char* sql = R"(
        SELECT client_id, client_name, device_type, client_ip, registration_timestamp, last_seen, metadata
        FROM clients;
    )";

    sqlite3_stmt* stmt;
    std::vector<Client> clients;

    if (sqlite3_prepare_v2(connection->getRaw(), sql, -1, &stmt, nullptr) != SQLITE_OK)
        return clients;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Client client;
        client.setId(sqlite3_column_int(stmt, 0));
        client.setName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        client.setDeviceType(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        client.setIp(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        client.setRegistrationTime(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));

        const char* lastSeen = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        client.setLastSeen(lastSeen ? std::optional<std::string>{lastSeen} : std::nullopt);

        const char* metadata = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        client.setMetadata(metadata ? std::optional<std::string>{metadata} : std::nullopt);

        clients.push_back(client);
    }

    sqlite3_finalize(stmt);
    return clients;
}

bool ClientRepository::update(const Client& client) {
    const char* sql = R"(
        UPDATE clients SET client_name = ?, device_type = ?, client_ip = ?, last_seen = ?, metadata = ?
        WHERE client_id = ?;
    )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(connection->getRaw(), sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, client.getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, client.getDeviceType().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, client.getIp().c_str(), -1, SQLITE_TRANSIENT);

    if (client.getLastSeen())
        sqlite3_bind_text(stmt, 4, client.getLastSeen()->c_str(), -1, SQLITE_TRANSIENT);
    else
        sqlite3_bind_null(stmt, 4);

    if (client.getMetadata())
        sqlite3_bind_text(stmt, 5, client.getMetadata()->c_str(), -1, SQLITE_TRANSIENT);
    else
        sqlite3_bind_null(stmt, 5);

    sqlite3_bind_int(stmt, 6, client.getId());

    bool success = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return success;
}

bool ClientRepository::remove(int id) {
    const char* sql = "DELETE FROM clients WHERE client_id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(connection->getRaw(), sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_int(stmt, 1, id);
    bool success = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return success;
}
