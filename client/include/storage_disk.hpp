#ifndef STORAGE_DISK_HPP
#define STORAGE_DISK_HPP

#include <iostream>
#include "types.hpp"
#include "horcrux.hpp"
#include "connection.hpp"
#include <vector>
#include <boost/asio.hpp>
#include "storage_disk.hpp"
#include "client.hpp"
#include <fstream>
#include <cstdlib>
#include <filesystem>
#include <uuid/uuid.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
using namespace std;
namespace fs = std::filesystem;

template <typename T>
class Storage : public Horcrux<T> {
};

struct passwd *pw = getpwuid(getuid());

const char *homedir = pw->pw_dir;

class Disk: Storage<Client&> {
    public:
        int save (string path, int n_horcruxes, std::string checksum, Client& client) override {
            std::unordered_map<std::string, std::string> envMap = loadEnv("/home/calogero/cubbit/.env");
            string lines;
            ifstream file (path, ios::binary | ios::ate);
            streamsize size = file.tellg();

            file.seekg(0, ios::beg);

            string id = get_uuid();
            string home = homedir;
            string dir_path = home + "/" + id;

            vector<unsigned char> ubuffer = getUnsignedCharFile(path);

            vector<char> buffer (size);
            int chunk_size = size / n_horcruxes;
            if (file.read(buffer.data (), size)) {
                vector<unsigned char> hash = calculateSHA256(ubuffer);
                string checksum = convertToHex(hash);
                string message = checksum + ";" + "checksum" + ";" + id ;
                client.sendData(message);
                for(int i = 0; i < n_horcruxes; i++) {
                    vector<char> chunk;
                    for (int j = i * chunk_size; j < (i+1) * chunk_size && j < buffer.size(); j++) {
                        chunk.push_back(buffer[j]);
                    }
                    
                    
                    std::string str_data(chunk.begin(), chunk.end());
                    string message = str_data + ";" + to_string(i) + ";" + id ;
                    client.sendData(message);

                }

                cout<<"Id of saved file: "<<id<<endl;
                cout<<"Folder of saved horcrux: "<<dir_path<<endl;
                
            } else {
                std::cout << "Error in reading file" << std::endl; 
            }

            return 0;

        }
        std::vector<load_response> load(std::string id, std::string path, Client& client) override{
            std::vector<load_response> result;
            client.receiveData(id, path);

            return result;
        }
};

#endif 