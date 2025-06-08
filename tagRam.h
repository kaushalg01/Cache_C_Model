#pragma once
#include "tagElement.h"
#include <vector>
#include <stdexcept>
class tagRam {
public:
    address_t numSets;
    address_t associativity;
    std::vector<std::vector<tagElement>> Ram;
    tagRam(address_t numSets, address_t associativity);
    ~tagRam();
    void setTag(address_t setIndex, address_t wayIndex, address_t tag);
    void setValid(address_t setIndex, address_t wayIndex, bool valid);
    void setDirty(address_t setIndex, address_t wayIndex, bool dirty);
    tagElement getTagElement(address_t setIndex, address_t wayIndex);
    address_t getTag(address_t setIndex, address_t wayIndex);
    bool getValid(address_t setIndex, address_t wayIndex);
    bool getDirty(address_t setIndex, address_t wayIndex);
};