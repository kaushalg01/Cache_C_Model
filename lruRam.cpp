#include <iostream>
#include <vector>
using namespace std;
#include "lruRam.h"
typedef unsigned int address_t;

lruRam::lruRam(address_t numSets, address_t associativity)
    : numSets(numSets), associativity(associativity) {
    LRU.resize(numSets);
    for (address_t i = 0; i < numSets; ++i) {
        LRU[i].resize(associativity);
        for(address_t j = 0; j < associativity; ++j) {
            LRU[i][j] = j; // Initialize LRU indices, way 0 is MRU, way N-1 is LRU having the highest/worst LRU value
        }
    }
}
void lruRam::updateLRU(address_t setIndex, address_t wayIndex) {
    if (setIndex < 0 || setIndex >= numSets || wayIndex < 0 || wayIndex >= associativity) {
        cout << "setValid called with setIndex: " << setIndex << ", wayIndex: " << wayIndex << endl;
        throw out_of_range("Invalid set or way index in updateLRU");
    }
    // Move the accessed way to the MRU position
    address_t lruValue = LRU[setIndex][wayIndex];
    for (address_t i = 0; i < associativity; ++i) {
        if (LRU[setIndex][i] < lruValue) {
            LRU[setIndex][i]++;
        }
    }
    LRU[setIndex][wayIndex] = 0; // Set the accessed way to MRU
}
address_t lruRam::getLRUIndex(address_t setIndex) {
    if (setIndex < 0 || setIndex >= numSets) {
        cout << "setValid called with setIndex: " << setIndex << ", wayIndex: " << setIndex << endl;
        throw out_of_range("Invalid set index in getLRUIndex");
    }
    // Find the way with the highest LRU value (the least recently used)
    address_t lruWay = 0;
    for (address_t i = 1; i < associativity; ++i) {
        if (LRU[setIndex][i] > LRU[setIndex][lruWay]) {
            lruWay = i;
        }
    }
    return lruWay; // Return the index of the least recently used way
}
void lruRam::resetLRU(address_t setIndex) {
    if (setIndex < 0 || setIndex >= numSets) {
        cout << "setValid called with setIndex: " << setIndex << ", wayIndex: " << setIndex << endl;
        throw out_of_range("Invalid set index in resetLRU");
    }
    // Reset LRU values for the specified set
    for (address_t i = 0; i < associativity; ++i) {
        LRU[setIndex][i] = i; // Reset to initial state
    }
}
lruRam::~lruRam() {
    // Destructor to clean up if necessary
}
void lruRam::printLRU() {
    for (address_t i = 0; i < numSets; ++i) {
        cout << "Set " << i << ": ";
        for (address_t j = 0; j < associativity; ++j) {
            cout << LRU[i][j] << " ";
        }
        cout << endl;
    }
}