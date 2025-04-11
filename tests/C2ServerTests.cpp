#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/C2Server.cpp"
#include "../include/ConfigManager.h"

using ::testing::Return;
using ::testing::_;
using ::testing::HasSubstr;


class C2ServerTestSuite : public ::testing::Test {
protected:
    void SetUp() override {
        ConfigManager::Instance().Load("test_config.env");
    }
    void simulateUserInput(const std::string& input) {
        originalCinBuffer = std::cin.rdbuf();
        fakeInputStream.str(input);
        fakeInputStream.clear();  // Clear flags in case it’s been used
        std::cin.rdbuf(fakeInputStream.rdbuf());
    }

    void restoreCin() {
        std::cin.rdbuf(originalCinBuffer);
    }

    std::streambuf* originalCinBuffer;
    std::istringstream fakeInputStream;
};

class MockSocket : public ISocket {
public:
    MOCK_METHOD(void, bindSocket, (int), (override));
    MOCK_METHOD(void, listenToSocket, (), (override));
    MOCK_METHOD(int, acceptConnection, (), (override));
    MOCK_METHOD(void, sendToClient, (int, const std::string&), (override));
    MOCK_METHOD(std::string, receiveFromClient, (int), (override));
    MOCK_METHOD(void, closeConnection, (), (override));
    MOCK_METHOD(void, closeClientConnection, (int), (override));

};

TEST_F(C2ServerTestSuite, ShouldBindAndListenWithoutError) {
    auto mockSocket = std::make_unique<MockSocket>();
    MockSocket* rawSocket = mockSocket.get();

    const int testClientSocket = 1;

    EXPECT_CALL(*rawSocket, bindSocket(ConfigManager::Instance().Get<int>("port"))).Times(1);
    EXPECT_CALL(*rawSocket, listenToSocket()).Times(1);
    EXPECT_CALL(*rawSocket, acceptConnection()).WillOnce(Return(testClientSocket));
    EXPECT_CALL(*rawSocket, closeClientConnection(testClientSocket)).Times(1);
    EXPECT_CALL(*rawSocket, closeConnection()).Times(1);

    simulateUserInput("exit\n");
    C2Server server(std::move(mockSocket));
    server.enableOneShotMode();
    server.run();
    restoreCin();
}

TEST_F(C2ServerTestSuite, ShouldHandleReceiveErrorGracefully) {
    auto mockSocket = std::make_unique<MockSocket>();
    MockSocket* rawSocket = mockSocket.get();

    const int testClientSocket = 1;

    EXPECT_CALL(*rawSocket, bindSocket(ConfigManager::Instance().Get<int>("port"))).Times(1);
    EXPECT_CALL(*rawSocket, listenToSocket()).Times(1);
    EXPECT_CALL(*rawSocket, acceptConnection()).WillOnce(Return(testClientSocket));
    EXPECT_CALL(*rawSocket, sendToClient(testClientSocket, HasSubstr("whoami"))).Times(1);
    EXPECT_CALL(*rawSocket, receiveFromClient(testClientSocket)).WillOnce(testing::Throw(std::runtime_error("Receive failed")));
    EXPECT_CALL(*rawSocket, closeClientConnection(testClientSocket)).Times(1);
    EXPECT_CALL(*rawSocket, closeConnection()).Times(1);
    
    simulateUserInput("whoami\n");
    C2Server server(std::move(mockSocket));
    server.enableOneShotMode();
    server.run();
    restoreCin();
}
