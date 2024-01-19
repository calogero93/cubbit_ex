#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include "base.hpp"
#include "utils.hpp"
#include <filesystem>
#include <pwd.h>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>

namespace fs = std::filesystem;
using namespace std;
struct passwd *pw = getpwuid(getuid());

const char *homedir = pw->pw_dir;

class TcpSession : public Session {
public:
    explicit TcpSession(boost::asio::ip::tcp::socket socket)
        : Session(std::move(socket)) {}

    void start() override {
        readData();
    }

private:
    void readData() {
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(data_),
            [this, self](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    std::string message = std::string(data_, length);
                    std::vector<std::string> splitted_message = splitStringServer(message, ';');
                    handleRequest(splitted_message, length);

                    readData();
                }
    
            }
        );
    }

    void handleRequest(std::vector<std::string> request, std::size_t length) {
         auto self(shared_from_this());
        if (request[0] == "SEND_HORCRUX") {
            request.erase(request.begin());
            processData(request, length);
        } else if (request[0] == "GET_HORCRUXES") {
            returnData(request[1], length);
        } else {
            std::cerr << "Unknown request:" << request[0] << std::endl;
        }
    }

    void processData(const std::vector<std::string> splitted_data, std::size_t length) {
        string id = splitted_data[2];
        string home = homedir;
        string dir_path = home + "/" + id;

        std::cout << "Received message from client: " << splitted_data[0] << std::endl;
        if (!fs::exists(dir_path)) {
            fs::create_directory(dir_path);
        }
        

        string index = splitted_data[1];
        string horcrux_path = dir_path + "/" + index + ".bin";
        ofstream out_file(horcrux_path, ios::binary | ios::out);

        if (out_file.is_open()) {
            out_file.write(splitted_data[0].c_str(), splitted_data[0].length());
        }

        boost::asio::write(socket_, boost::asio::buffer("OK"));
      
    }

    void returnData(std::string id, std::size_t length) {
        auto self(shared_from_this());
        string home = homedir;
        string dir_path = home + "/" + id.c_str();
        int n_horcruxes = countFilesInFolderServer(dir_path) - 1;
        if (n_horcruxes == 0) {
            boost::asio::write(socket_, boost::asio::buffer("FAIL;"));
        }
        for (const auto& entry : fs::directory_iterator(dir_path)) {
            std::ifstream file(entry.path().string());
            std::cout <<dir_path << std::endl;
            if (fs::is_regular_file(entry.path())) {
                
                std::string index = entry.path().stem().string();
                std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                boost::asio::write(socket_, boost::asio::buffer("OK;" + index + ";" + to_string(n_horcruxes) + ";" + content));
            }           
        }
        socket_.close();
    }


    void writeData(std::size_t length) {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
            [this, self](boost::system::error_code ec, std::size_t) {
                if (!ec) {
                    readData();
                }
            }
        );
    }

    enum { max_length = 1024 };
    char data_[max_length];
};

class TcpServer : public Server {
public:
    TcpServer(boost::asio::io_context& io_context, short port)
        : Server(io_context, port) {}

private:
    std::shared_ptr<Session> createSession(boost::asio::ip::tcp::socket socket) override {
        return std::make_shared<TcpSession>(std::move(socket));
    }
};