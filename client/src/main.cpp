#include <iostream>
#include <vector>
#include "../include/storage_disk.hpp"

void processCommand(const std::string& option, const std::string& value) {
    std::cout << "Option: " << option << ", Value: " << value << std::endl;
}

int main (int argc, char** argv) {

    boost::asio::io_context ioContext;
    Disk disk;
    TcpClient client(ioContext, "127.0.0.1", "8003");
    ioContext.run();

    std::string comando(argv[1]);

    if (comando == "save") {
        if (argc != 5 || std::string(argv[2]) != "-n") {
            std::cerr << "Usage: " << argv[0] << " save -n <horcrux count> <input path>" << std::endl;
            return 1;
        }
        int horcruxCount = std::stoi(argv[3]);
        std::string inputPath(argv[4]);
        disk.save(inputPath, horcruxCount, "", client);
    } else if (comando == "load") {
        if (argc != 4) {
            std::cerr << "Usage: " << argv[0] << " load <file_id> <output_file>" << std::endl;
            return 1;
        }
         std::string fileId = argv[2];
        std::string outputPath(argv[3]);
        disk.load(fileId, outputPath, client);
    } else {
        std::cerr << "Comando sconosciuto: " << comando << std::endl;
        return 1;
    }

    
} 