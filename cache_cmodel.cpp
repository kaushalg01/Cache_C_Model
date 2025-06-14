#include "cache_cmodel.h" // Ensure this header declares class Cache and address_t

Cache::Cache(address_t numSets, address_t blockSize, address_t cacheSize, address_t associativity)
    : numSets(numSets),
      blockSize(blockSize),
      cacheSize(cacheSize),
      associativity(associativity),
      T(numSets, associativity),
      L(numSets, associativity) {
    if (numSets <= 0 || blockSize <= 0 || cacheSize <= 0 || associativity <= 0) {
        throw invalid_argument("Cache parameters must be positive integers.");
    }
    blockOffsetBits = log2(blockSize);
    setIndexBits = log2(numSets);
    tagBits = 32 - (blockOffsetBits + setIndexBits); // Assuming address_t is 32 bits
}

void Cache::insert(address_t addr, address_t wayIndex) {

  address_t offsetNumb = addr & ((1 << blockOffsetBits) - 1);
  address_t setNumb = (addr & (((1 << ((setIndexBits + blockOffsetBits))) - 1) - (((1 << blockOffsetBits )) - 1))) >> blockOffsetBits; 
  address_t tagNumb = (addr & ((((1 << (tagBits + setIndexBits + blockOffsetBits))) - 1) - (((1 << (setIndexBits + blockOffsetBits))) - 1))) >> (setIndexBits + blockOffsetBits);
    if (setNumb < 0 || setNumb >= numSets || wayIndex < 0 || wayIndex >= associativity) {
        cout << "setValid called with setIndex: " << setNumb << ", wayIndex: " << wayIndex << endl;
        throw out_of_range("Invalid set or way index in insert");
    }
    T.setTag(setNumb, wayIndex, tagNumb);
    T.setValid(setNumb, wayIndex, true);
    T.setDirty(setNumb, wayIndex, false); // Assuming new insertions are not dirty
    L.updateLRU(setNumb, wayIndex); // Update LRU order
}

bool Cache::isHit(address_t addr, address_t* way_hit)
{ 
  address_t offsetNumb = addr & ((1 << blockOffsetBits) - 1);
  address_t setNumb = (addr & (((1 << ((setIndexBits + blockOffsetBits))) - 1) - (((1 << blockOffsetBits )) - 1))) >> blockOffsetBits; 
  address_t tagNumb = (addr & ((((1 << (tagBits + setIndexBits + blockOffsetBits))) - 1) - (((1 << (setIndexBits + blockOffsetBits))) - 1))) >> (setIndexBits + blockOffsetBits);
  cout << "OffsetNumb " << hex << offsetNumb << endl;
  cout << "setNumb " << hex << setNumb <<endl;
  for (address_t ii = 0; ii < associativity; ii++) {
        if (T.getValid(setNumb, ii) && T.getTag(setNumb, ii) == tagNumb) {
            L.updateLRU(setNumb, ii); // Update LRU order
            *way_hit = ii; // Store the way index of the hit
            return true; // Cache hit
        }
    }
    return false; // Cache miss
}
address_t Cache::evictWay(address_t addr)
{

  address_t offsetNumb = addr & ((1 << blockOffsetBits) - 1);
  address_t setNumb = (addr & (((1 << ((setIndexBits + blockOffsetBits))) - 1) - (((1 << blockOffsetBits )) - 1))) >> blockOffsetBits; 
  address_t tagNumb = (addr & ((((1 << (tagBits + setIndexBits + blockOffsetBits))) - 1) - (((1 << (setIndexBits + blockOffsetBits))) - 1))) >> (setIndexBits + blockOffsetBits);
    
    // Find the least recently used way in the set
    address_t lruWay = L.getLRUIndex(setNumb);
    
    // Evict the least recently used way
    T.setValid(setNumb, lruWay, true);
    T.setTag(setNumb, lruWay, tagNumb); // Update tag
    T.setDirty(setNumb, lruWay, false); // Assuming eviction does not dirty the cache line
    L.updateLRU(setNumb, lruWay); // Update LRU order
    return lruWay; // Return the index of the evicted way
}
// This function replaces the address in the cache with a new tag.
// It checks if the address exists in the cache and updates the tag and dirty bit accordingly.
// If the address does not exist, it returns false.
// If the address exists, it updates the tag and LRU order, returning true.
address_t Cache::replaceAddr(address_t addr, address_t newAddr, address_t setNumb, int* flag)
{
    address_t old_tagNumb = (addr & (((1 << (tagBits + setIndexBits + blockOffsetBits)) - 1) - ((1 << (setIndexBits + blockOffsetBits)) - 1))) >> (setIndexBits + blockOffsetBits);
    address_t new_tagNumb = (newAddr & (((1 << (tagBits + setIndexBits + blockOffsetBits)) - 1) - ((1 << (setIndexBits + blockOffsetBits)) - 1))) >> (setIndexBits + blockOffsetBits);
    for (address_t ii = 0; ii < associativity; ii++) {
        if (T.getValid(setNumb, ii) && T.getTag(setNumb, ii) == old_tagNumb) {
            T.setTag(setNumb, ii, new_tagNumb); // Update the tag
            T.setValid(setNumb, ii, true); // Ensure the way is valid
            // TODO:: // If the way is dirty, we might need to handle write-back logic here
            // For simplicity, we assume the way is not dirty on replacement
            T.setDirty(setNumb, ii, false); // Assuming new insertions are not dirty
            L.updateLRU(setNumb, ii); // Update LRU order
            cout << "Replaced address in cache with new tag: " << hex << new_tagNumb << " at way index: " << ii << endl;
            cout << "Old tag: " << hex << old_tagNumb << " replaced with new tag: " << hex << new_tagNumb << endl;
            cout << "Set number: " << setNumb << endl;
            *flag = 1;
            return ii;
        }
    }
    *flag = 0;
    return 0; // Replacement failed, address not found
}

address_t Cache::freeWayExists(address_t addr, int *flag) {
  address_t offsetNumb = addr & ((1 << blockOffsetBits) - 1);
  address_t setNumb = (addr & (((1 << ((setIndexBits + blockOffsetBits))) - 1) - (((1 << blockOffsetBits )) - 1))) >> blockOffsetBits; 
  address_t tagNumb = (addr & ((((1 << (tagBits + setIndexBits + blockOffsetBits))) - 1) - (((1 << (setIndexBits + blockOffsetBits))) - 1))) >> (setIndexBits + blockOffsetBits);

  cout << "In free way exists function" << endl;
  cout << "OffsetNumb " << hex << offsetNumb << endl;
  cout << "setNumb " << hex << setNumb << endl;
  cout << "tagNumb " << hex << tagNumb << endl;
  cout <<endl;
    for (address_t ii = associativity - 1 ; ii != 0XFFFFFFFF; ii--) {
        if (!T.getValid(setNumb, ii)) {
            *flag = 1;
            return ii; // Return the index of the first free way
        }
    }
    *flag = 0;
    return 0; // No free way exists
}

void Cache::printCacheState(){
    cout << "Cache State:" << endl;
    for (address_t i = 0; i < numSets; ++i) {
        cout << "Set " << i << ": ";
        for (address_t j = 0; j < associativity; ++j) {
            if (T.getValid(i, j)) {
                cout << "Way " << j << " - Tag: " << hex << T.getTag(i, j) 
                     << ", Dirty: " << T.getDirty(i, j) << ", Valid: " << T.getValid(i, j) << " | ";
            } else {
                cout << "Way " << j << " - Invalid | ";
            }
        }
        cout << endl;
    }
}
Cache::~Cache() {
    // Destructor to clean up if necessary
    // In this case, we don't have dynamic memory allocation, so nothing specific to do
}
// The destructor will automatically clean up the tagRam and lruRam objects