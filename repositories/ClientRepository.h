#pragma once
#include "../include/IRepository.h"
#include "../models/Client.h"
#include "../include/SQLiteConnection.h"

class ClientRepository : public IRepository<Client> {
public:
    explicit ClientRepository(SQLiteConnection* conn);

    bool create(const Client& client) override;
    std::optional<Client> getById(int id) override;
    std::vector<Client> getAll() override;
    bool update(const Client& client) override;
    bool remove(int id) override;

private:
    SQLiteConnection* connection;
};
