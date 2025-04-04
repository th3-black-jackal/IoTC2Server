#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/C2Server.cpp"

using ::testing::Return;
using ::testing::_;




class MockSocket : public ISocket {
    
public:
    MOCK_METHOD(void, bindSocket, (int), (override));
    MOCK_METHOD(void, listenToSocket, (), (override));
    MOCK_METHOD(int, acceptConnection, (), (override));
    MOCK_METHOD(void, sendToClient, (int, const std::string&), (override));
    MOCK_METHOD(std::string, receiveFromClient, (int), (override));
    MOCK_METHOD(void, closeConnection, (), (override));
};

TEST(C2ServerTest, ShouldBindAndListenWithoutError) {
    auto mockSocket = std::make_unique<MockSocket>();
    MockSocket* rawSocket = mockSocket.get();

    EXPECT_CALL(*rawSocket, bindSocket(PORT)).Times(1);
    EXPECT_CALL(*rawSocket, listenToSocket()).Times(1);
    EXPECT_CALL(*rawSocket, acceptConnection()).WillOnce(Return(1));
    EXPECT_CALL(*rawSocket, receiveFromClient(_)).WillOnce(Return("exit"));
    EXPECT_CALL(*rawSocket, closeConnection()).Times(1);
    EXPECT_CALL(*rawSocket, sendToClient(_, _)).Times(1);

    C2Server server(std::move(mockSocket));

    std::istringstream fakeInput("exit\n");
    std::cin.rdbuf(fakeInput.rdbuf());

    server.run();
}

TEST(C2ServerTest, ShouldHandleReceiveErrorGracefully) {
    auto mockSocket = std::make_unique<MockSocket>();
    MockSocket* rawSocket = mockSocket.get();

    EXPECT_CALL(*rawSocket, bindSocket(PORT)).Times(1);
    EXPECT_CALL(*rawSocket, listenToSocket()).Times(1);
    EXPECT_CALL(*rawSocket, acceptConnection()).WillOnce(Return(1));
    EXPECT_CALL(*rawSocket, sendToClient(_, _)).Times(1);
    EXPECT_CALL(*rawSocket, receiveFromClient(_)).WillOnce(testing::Throw(std::runtime_error("Receive failed")));
    EXPECT_CALL(*rawSocket, closeConnection()).Times(1);

    C2Server server(std::move(mockSocket));
    std::istringstream fakeInput("whoami\n");
    std::cin.rdbuf(fakeInput.rdbuf());

    server.run();
}
