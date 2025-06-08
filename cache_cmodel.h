#include "lruRam.h"
#include "tagRam.h"
#include <iostream>
#include <vector>
typedef unsigned int address_t; // Define address_t as an unsigned int

using namespace std;
class Cache {
    public:
        address_t numSets;
        address_t blockSize;
        address_t cacheSize;
        address_t associativity;
        address_t blockOffsetBits;
        address_t setIndexBits;
        address_t tagBits;
        tagRam T;
        lruRam L;
        Cache(address_t numSets, address_t blockSize, address_t cacheSize, address_t associativity);
        void insert(address_t setIndex, address_t wayIndex);
        bool isHit(address_t addr, address_t* way_hit);
        address_t evictWay(address_t addr);
        // This function replaces the address in the cache with a new tag.
        // It checks if the address exists in the cache and updates the tag and dirty bit accordingly.
        // If the address does not exist, it returns false.
        // If the address exists, it updates the tag and LRU order, returning true.
        address_t replaceAddr(address_t addr, address_t newAddr, address_t setNumb, int* flag);
        address_t freeWayExists(address_t addr);
        void printCacheState();
        ~Cache();
};