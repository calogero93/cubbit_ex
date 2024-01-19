#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <openssl/sha.h>
#include <uuid/uuid.h>
#include <vector>
namespace fs = std::filesystem;
using namespace std;

std::unordered_map<std::string, std::string> loadEnvServer(const std::string& filePath) {
    std::unordered_map<std::string, std::string> envMap;
    std::ifstream file(filePath);

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string key, value;

            if (std::getline(iss, key, '=')) {
                if (std::getline(iss, value)) {
                    envMap[key] = value;
                }
            }
        }
        file.close();
    } else {
        std::cerr << "Impossibile aprire il file .env" << std::endl;
    }

    return envMap;
}

vector<std::string> splitStringServer(const std::string& input, char delimiter) {
    std::vector<std::string> parts;

    size_t start = 0;
    size_t end = input.find(delimiter);

    while (end != std::string::npos) {
        parts.push_back(input.substr(start, end - start));
        start = end + 1;
        end = input.find(delimiter, start);
    }

    // Add the last part
    parts.push_back(input.substr(start));
    return parts;
}

std::vector<unsigned char> calculateSHA256Server(const std::vector<unsigned char>& data) {
    std::vector<unsigned char> hash(SHA256_DIGEST_LENGTH, 0);

    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, data.data(), data.size());
    SHA256_Final(hash.data(), &sha256);

    return hash;
}

std::vector<unsigned char> getUnsignedCharFileServer(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return {};
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer(size);
    if (file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        return buffer;
    } else {
        std::cerr << "Error reading file: " << filePath << std::endl;
        return {};
    }
}

std::string convertToHexServer(const std::vector<unsigned char>& data) {
    std::stringstream hexStringStream;
    hexStringStream << std::hex << std::setfill('0');
    
    for (const auto& byte : data) {
        hexStringStream << std::setw(2) << static_cast<int>(byte);
    }

    return hexStringStream.str();
}

int countFilesInFolderServer(const std::string& folderPath) {
    int count = 0;

    try {
        for (const auto& entry : fs::directory_iterator(folderPath)) {
            if (entry.is_regular_file()) {
                ++count;
            }
        }
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }

    return count;
}