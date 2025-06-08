#include <iostream>
#include <vector>
typedef unsigned int address_t; // Define address_t as an unsigned int
#include <stdexcept>

using namespace std;

class tagElement {
    public:
        // This defines the structure of a tagElement
            address_t tag;
            bool valid;
            bool dirty;

            tagElement() : tag(0), valid(false), dirty(false) {}
};