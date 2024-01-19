#include <iostream>
#include "server.hpp"
using namespace std;


int main (int argc, char** argv) {
     try {
        boost::asio::io_context ioContext;
        TcpServer server(ioContext, 8003);
        ioContext.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
} 