#ifndef MAIN_HPP
#define MAIN_HPP

#include <iostream>
#include "types.hpp"
#include "connection.hpp"
#include <vector>
using namespace std;

template <typename T>
class Horcrux {
    public:
        virtual int save (string path, int n_horcruxes, string checksum, T connection) = 0;
        virtual vector<load_response> load(string i, string path,  T connection) = 0;
};



#endif 