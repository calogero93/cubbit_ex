#include "../client/include/client.hpp"
#include "../server/server.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

class MockClient : public Client {
public:
    MockClient(boost::asio::io_context& io_context,
               const std::string& serverAddress,
               const std::string& serverPort)
        : Client(io_context, serverAddress, serverPort) {}

    MOCK_METHOD(void, sendData, (const std::string&), (override));
    MOCK_METHOD(void, receiveData, (std::string, const std::string&), (override));
};

class MockServer : public Server {
public:
    MockServer(boost::asio::io_context& io_context,
               short serverPort)
        : Server(io_context, serverPort) {}

    MOCK_METHOD(void, readData, (), ());
};

TEST(TcpClientIntegrationTest, AsyncSendAndReceive) {
    boost::asio::io_context ioContext;

    TcpClient realClient(ioContext, "127.0.0.1", "12345");
    MockClient mockClient(ioContext, "127.0.0.1", "12345");
    TcpServer realServer(ioContext, 12345);

    EXPECT_CALL(mockClient, sendData("test_data"))
        .WillOnce(testing::Invoke([&](const std::string& data) {
            ioContext.post([&, data]() {
                realClient.sendData("test_data");
            });
        }));

    mockClient.sendData("test_data");
    ioContext.run();

}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
