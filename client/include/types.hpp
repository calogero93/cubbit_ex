#ifndef TYPES_HPP
#define TYPES_HPP

#include <vector>
#include <iostream>

struct horcrux {
    std::vector<char> content;
};

struct load_response {
    int horcrux_index;
    int horcruxes;
    int status_code;
    horcrux content; 
};

#endif 