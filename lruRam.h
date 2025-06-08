#include <iostream>
#include <vector>
using namespace std;
typedef unsigned int address_t;

class lruRam {
    public:
        vector<vector<int>> LRU;
        address_t numSets; // Number of sets in the cache
        address_t associativity; // Number of ways in the cache
        lruRam(address_t numSets, address_t associativity);
        void updateLRU(address_t setIndex, address_t wayIndex);
        address_t getLRUIndex(address_t setIndex);
        void resetLRU(address_t setIndex);
        ~lruRam();
        void printLRU();
};