#include <iostream>
#include <memory>
#include <string>
#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace http = boost::beast::http;

class Session : public std::enable_shared_from_this<Session> {
public:
    explicit Session(boost::asio::ip::tcp::socket socket)
        : socket_(std::move(socket)) {}

    virtual void start() = 0;

protected:
    boost::asio::ip::tcp::socket socket_;
    boost::beast::flat_buffer buffer_;
};

// Base class for servers
class Server {
public:
    Server(boost::asio::io_context& io_context, short port)
        : io_context_(io_context),
          acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
          socket_(io_context) {
        startAccept();
    }

    virtual ~Server() = default;

protected:
    virtual std::shared_ptr<Session> createSession(boost::asio::ip::tcp::socket socket) = 0;

    void startAccept() {
        acceptor_.async_accept([this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
            if (!ec) {
                createSession(std::move(socket))->start();
            }
            startAccept();
        });
    }

    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::socket socket_;
};