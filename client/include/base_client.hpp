#include <iostream>
#include <boost/asio.hpp>


class Client {
public:
    Client(boost::asio::io_context& io_context, const std::string& serverAddress, const std::string& serverPort)
        : io_context_(io_context), serverAddress_(serverAddress), serverPort_(serverPort) {
    }

    virtual void sendData(const std::string& data) = 0;

    virtual void receiveData(std::string id, const std::string& path) = 0;

protected:
    boost::asio::io_context& io_context_;
    std::string serverAddress_;
    std::string serverPort_;
};