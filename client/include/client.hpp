#ifndef CLIENT_HPP
#define CLIENT_DISK_HPP

#include <vector>
#include "base_client.hpp"
#include "utils.hpp"

class TcpClient : public Client {
public:
    TcpClient(boost::asio::io_context& io_context, const std::string& serverAddress, const std::string&  serverPort)
    : Client(io_context, serverAddress, serverPort), socket_(io_context), resolver_(io_context) {
        auto endpoints = resolver_.resolve( boost::asio::ip::tcp::v4(), serverAddress, serverPort);
        boost::asio::connect(socket_, endpoints);
    }  

    void sendData(const std::string& data) override {
        try {
            boost::asio::write(socket_, boost::asio::buffer("SEND_HORCRUX;" + data));
            std::array<char, 1024> response;
        boost::system::error_code error;


        size_t bytesRead = socket_.read_some(boost::asio::buffer(response), error);

        if (error == boost::asio::error::eof) {
            std::cout << "Connection closed from server" << std::endl;
        } else if (error) {
            throw boost::system::system_error(error);
        }

        std::cout << "Reply from server: " << std::string(response.data(), bytesRead) << std::endl;

        } catch (const std::exception& e) {
            std::cerr << "Exception during send message: " << e.what() << std::endl;
        }
    }

    void receiveData(std::string id, const std::string& path) override {
    
         boost::asio::write(socket_, boost::asio::buffer("GET_HORCRUXES;" + id));
         std::ofstream file(path, ios::binary | ios::out);
         string joined_file = "";
         string checksum = "";

        while (true) {
            std::array<char, 1024> reply;
            boost::system::error_code error;

        
            size_t reply_length = socket_.read_some(boost::asio::buffer(reply), error);

            if (error == boost::asio::error::eof || reply_length == 0) {
                
                break;
            } else if (error) {
                throw boost::system::system_error(error);
            }

            string response = std::string(reply.data(), reply_length);

            vector<string> splitted_response = splitString(response, ';');

            if (splitted_response[0] == "FAIL") {
                std::cerr<<"The horcrux is not found"<<endl;
                return;
            } else {
                if (splitted_response[1] == "checksum") {
                    checksum +=  splitted_response[3];
                } else {
                    joined_file += splitted_response[3];
                }
               
            }

             std::cout << "Reply from server: " << std::string(reply.data(), reply_length) << std::endl;

        }

        if (file.is_open()) {
            file.write(joined_file.c_str(), joined_file.length());
        }

        vector<unsigned char> ubuffer = getUnsignedCharFile(path);
        vector<unsigned char> hash = calculateSHA256(ubuffer);
        string file_checksum = convertToHex(hash);
        cout<<file_checksum;
            if(file_checksum != checksum) {
            std::cerr<<"Checksum check failed"<<endl;
            return;
        }
            
        return;
    }

    void receiveMessage( boost::asio::ip::tcp::socket& socket) {
        cout<<socket.is_open();
        vector<char> reply;
        for (int i = 0; i<6; i++) {
            socket.read_some(boost::asio::buffer(reply));
        }
    }
    
    enum { max_length = 1024 };
    char data_[max_length];
    boost::asio::ip::tcp::resolver resolver_;
    boost::asio::ip::tcp::socket socket_;
};

#endif