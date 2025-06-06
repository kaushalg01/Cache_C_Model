#include "lruRam.cpp"
#include "tagRam.cpp"
#include <iostream>
#include <vector>

using namespace std;
class Cache {
    public:
        int numSets;
        int blockSize;
        int cacheSize;
        int associativity;
        int blockOffsetBits;
        int setIndexBits;
        int tagBits;
        tagRam T;
        lruRam L;
        Cache(int numSets, int blockSize, int cacheSize, int associativity)
            : numSets(numSets), blockSize(blockSize), cacheSize(cacheSize), associativity(associativity),
              T(numSets, associativity), L(numSets, associativity) {
            blockOffsetBits = log2(blockSize);
            setIndexBits = log2(numSets);
            tagBits = 32 - (blockOffsetBits + setIndexBits); // Assuming 32-bit addresses
        }
        void insert(int setIndex, int wayIndex, int tag);
        int isHit(int addr);
        int isEvictionNeeded(int addr);
        int replaceAddr(int addr, int replaceAddr_tag);
        ~Cache() {
            // Destructor to clean up if necessary
        }

};

void Cache::insert(int setIndex, int wayIndex, int tag) {
    
    if (setIndex < 0 || setIndex >= numSets || wayIndex < 0 || wayIndex >= associativity) {
        throw out_of_range("Invalid set or way index");
    }
    T[setIndex][wayIndex].tag = tag;
    T[setIndex][wayIndex].valid = true;
    T[setIndex][wayIndex].dirty = false; // Assuming new insertions are not dirty
    L.updateLRU(setIndex, wayIndex); // Update LRU order
}
int Cache::isHit(int addr)
{ 
  int offsetNumb = addr & ((1 << blockOffsetBits) - 1);
  int setNumb = addr & ((1 << ((setIndexBits + blockOffsetBits)) - 1) - ((1 << blockOffsetBits ) - 1)); 
  int tagNumb = addr & (((1 << (tagBits + setIndexBits + blockOffsetBits)) - 1) - ((1 << (setIndexBits + blockOffsetBits)) - 1));
   for (int ii = 0; ii < associativity; ii++) {
        if (T[setNumb][ii].valid && T[setNumb][ii].tag == tagNumb) {
            return true; // Cache hit
            L.updateLRU(setNumb, ii); // Update LRU order
        }
    }
    return false; // Cache miss
}
int Cache::isEvictionNeeded(int addr)
{
    int offsetNumb = addr & ((1 << blockOffsetBits) - 1);
    int setNumb = addr & ((1 << ((setIndexBits + blockOffsetBits)) - 1) - ((1 << blockOffsetBits ) - 1)); 
    for (int ii = 0; ii < associativity; ii++) {
        if (!T[setNumb][ii].valid) {
            return false; // No eviction needed, found an empty slot
        }
    }
    return true; // Eviction needed, all slots are valid
}
int Cache::replaceAddr(int addr, int replaceAddr_tag)
{
    int offsetNumb = addr & ((1 << blockOffsetBits) - 1);
    int setNumb = addr & ((1 << ((setIndexBits + blockOffsetBits)) - 1) - ((1 << blockOffsetBits ) - 1)); 
    int tagNumb = addr & (((1 << (tagBits + setIndexBits + blockOffsetBits)) - 1) - ((1 << (setIndexBits + blockOffsetBits)) - 1));
    for (int ii = 0; ii < associativity; ii++) {
        if (T[setNumb][ii].valid && T[setNumb][ii].tag == replaceAddr_tag) {
            T[setNumb][ii].tag = tagNumb;
            T[setNumb][ii].dirty = false; // Assuming new insertions are not dirty
            L.updateLRU(setNumb, ii); // Update LRU order
            return true; // Replacement successful
        }
    }
    return false; // Replacement failed, address not found
}
