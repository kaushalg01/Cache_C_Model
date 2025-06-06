#include <iostream>
#include <vector>

using namespace std;

class tagElement {
    public:
        // This defines the structure of a tagElement
            int tag;
            bool valid;
            bool dirty;

            tagElement() : tag(0), valid(false), dirty(false) {}
};