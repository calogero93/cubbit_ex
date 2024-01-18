#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <iostream>
#include "types.hpp"
#include <vector>
#include <boost/asio.hpp>
using namespace std;

template <typename T>
class Connection {
    virtual T get_connection() = 0;
};



#endif 