#include <gtest/gtest.h>
#include "../include/SQLiteConnection.h"
#include "../include/ConfigManager.h"
#include "../repositories/ClientRepository.h"
#include "../models/Client.h"

class ClientRepositoryTest : public ::testing::Test {
protected:
    SQLiteConnection connection;
    ClientRepository* repository;

    void SetUp() override {
        ConfigManager::Instance().Load("test_config.env");
        std::string database = ConfigManager::Instance().Get<std::string>("database");
        ASSERT_TRUE(connection.connect(database));
        repository = new ClientRepository(&connection);

        const char* createTableSQL = R"(
            CREATE TABLE clients (
                client_id INTEGER PRIMARY KEY,
                client_name TEXT NOT NULL UNIQUE,
                device_type TEXT,
                client_ip TEXT,
                registration_timestamp TEXT DEFAULT CURRENT_TIMESTAMP,
                last_seen TEXT,
                metadata TEXT
            );
        )";

        char* errorMsg = nullptr;
        int rc = sqlite3_exec(connection.getRaw(), createTableSQL, nullptr, nullptr, &errorMsg);
        ASSERT_EQ(rc, SQLITE_OK) << "Failed to create table: " << errorMsg;
        sqlite3_free(errorMsg);
    }

    void TearDown() override {
        delete repository;
        connection.disconnect();
    }
};

TEST_F(ClientRepositoryTest, CreateClient) {
    Client client;
    client.setName("testClient");
    client.setDeviceType("laptop");
    client.setIp("192.168.1.1");
    client.setLastSeen("2025-04-11T12:00:00Z");
    client.setMetadata("{\"os\":\"linux\"}");

    EXPECT_TRUE(repository->create(client));
}

TEST_F(ClientRepositoryTest, GetClientById) {
    Client client;
    client.setName("testClient");
    client.setDeviceType("phone");
    client.setIp("10.0.0.1");
    client.setLastSeen("2025-04-11T13:00:00Z");
    client.setMetadata("{\"os\":\"android\"}");

    ASSERT_TRUE(repository->create(client));

    auto results = repository->getAll();
    ASSERT_EQ(results.size(), 1);
    int insertedId = results[0].getId();

    auto fetched = repository->getById(insertedId);
    ASSERT_TRUE(fetched.has_value());
    EXPECT_EQ(fetched->getName(), "testClient");
    EXPECT_EQ(fetched->getDeviceType(), "phone");
    EXPECT_EQ(fetched->getIp(), "10.0.0.1");
}

TEST_F(ClientRepositoryTest, UpdateClient) {
    Client client;
    client.setName("initialName");
    client.setDeviceType("tablet");
    client.setIp("127.0.0.1");
    client.setMetadata(std::nullopt);
    client.setLastSeen(std::nullopt);

    ASSERT_TRUE(repository->create(client));

    auto inserted = repository->getAll()[0];
    inserted.setName("updatedName");
    inserted.setMetadata("{\"updated\":true}");

    EXPECT_TRUE(repository->update(inserted));

    auto updated = repository->getById(inserted.getId());
    ASSERT_TRUE(updated.has_value());
    EXPECT_EQ(updated->getName(), "updatedName");
    EXPECT_EQ(updated->getMetadata().value(), "{\"updated\":true}");
}

TEST_F(ClientRepositoryTest, DeleteClient) {
    Client client;
    client.setName("deletable");
    client.setDeviceType("router");
    client.setIp("172.16.0.1");

    ASSERT_TRUE(repository->create(client));

    auto inserted = repository->getAll()[0];
    EXPECT_TRUE(repository->remove(inserted.getId()));

    auto deleted = repository->getById(inserted.getId());
    EXPECT_FALSE(deleted.has_value());
}
