#include "tagRam.h"
#include <stdexcept>
typedef unsigned int address_t; // Define address_t as an unsigned int

tagRam::tagRam(address_t numSets, address_t associativity)
    : numSets(numSets), associativity(associativity) {
    Ram.resize(numSets);
    for (address_t i = 0; i < numSets; ++i) {
        Ram[i].resize(associativity);
    }
}
tagRam::~tagRam() {
    // Destructor to clean up if necessary
}

void tagRam::setTag(address_t setIndex, address_t wayIndex, address_t tag) {
    if (setIndex < 0 || setIndex >= numSets || wayIndex < 0 || wayIndex >= associativity) {
        throw out_of_range("Invalid set or way index");
    }
    Ram[setIndex][wayIndex].tag = tag;
}

void tagRam::setValid(address_t setIndex, address_t wayIndex, bool valid) {
    if (setIndex < 0 || setIndex >= numSets || wayIndex < 0 || wayIndex >= associativity) {
        throw out_of_range("Invalid set or way index");
    }
    Ram[setIndex][wayIndex].valid = valid;
}
void tagRam::setDirty(address_t setIndex, address_t wayIndex, bool dirty) {
    if (setIndex < 0 || setIndex >= numSets || wayIndex < 0 || wayIndex >= associativity) {
        throw out_of_range("Invalid set or way index");
    }
    Ram[setIndex][wayIndex].dirty = dirty;      
}
tagElement tagRam::getTagElement(address_t setIndex, address_t wayIndex) {
    if (setIndex < 0 || setIndex >= numSets || wayIndex < 0 || wayIndex >= associativity) {
        throw out_of_range("Invalid set or way index");
    }
    return Ram[setIndex][wayIndex];
}
address_t tagRam::getTag(address_t setIndex, address_t wayIndex) {
    if (setIndex < 0 || setIndex >= numSets || wayIndex < 0 || wayIndex >= associativity) {
        throw out_of_range("Invalid set or way index");
    }
    return  Ram[setIndex][wayIndex].tag;
}
bool tagRam::getValid(address_t setIndex, address_t wayIndex) {
    if (setIndex < 0 || setIndex >= numSets || wayIndex < 0 || wayIndex >= associativity) {
        throw out_of_range("Invalid set or way index");
    }
    return Ram[setIndex][wayIndex].valid;
}
bool tagRam::getDirty(address_t setIndex, address_t wayIndex) {
    if (setIndex < 0 || setIndex >= numSets || wayIndex < 0 || wayIndex >= associativity) {
        throw out_of_range("Invalid set or way index");
    }
    return Ram[setIndex][wayIndex].dirty;
}