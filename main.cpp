#include "cache_cmodel.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;
typedef unsigned int address_t; // Define address_t as an unsigned int

int main() {
    string line; // Variable to read lines from the input file
    cout << "Cache Simulation Started..." << endl;
    
    // Initialize random seed and cache parameters
    // Note: These values can be adjusted based on your specific cache configuration
    // For example, you might want to read these from a configuration file or command line arguments
    // srand(time(0)); // Uncomment this line if you want to use random numbers in your simulation
    // For now, we will not use random numbers in this example

srand(time(0)); // Seed for random number generation 
    address_t numSets = 16; // Example number of sets
    address_t associativity = 4; // Example associativity
    address_t blockSize = 64; // Example block size in bytes
    address_t cacheSize = numSets * blockSize * associativity; // Total cache size
    address_t way_hit = -1; // Variable to store the way index of a hit
    int flag = 0; // Flag to indicate if a replacement occurred
    vector<address_t> addr_acess;

    ifstream inputFile("input.txt");
    if (!inputFile) {
        cerr << "Error opening input file." << endl;
        return 1;
    }
    while (getline(inputFile, line)) {
        if (!line.empty()) {
            addr_acess.push_back(stoi(line, nullptr, 16)); // Convert hex string to integer
        }
    }
    inputFile.close();

    // Create a Cache object
    Cache myCache(numSets, blockSize, cacheSize, associativity);

    for (address_t ii = 0; ii < addr_acess.size(); ii++) {
        address_t addr = addr_acess[ii];
        cout << "Processing address: " << hex << addr << endl;
        if(myCache.isHit(addr_acess[ii], &way_hit)) {
            // Cache hit
            cout << "Cache hit for address: " << hex << addr_acess[ii] << "way hit" << way_hit << endl;
        } else 
        {
            // Cache miss
            cout << "Cache miss for address: " << hex << addr_acess[ii] << endl;
            cout << "Checking if free way exists..." << endl;
            address_t freeWay = myCache.freeWayExists(addr_acess[ii]);
            way_hit = freeWay;
            if (freeWay != -1) {
                // Free way exists, insert the address
                cout << "Free way found at index: " << freeWay << endl;
                myCache.insert(addr_acess[ii], freeWay);
            } else {
                // No free way, evict a way
                cout << "No free way available, evicting a way..." << endl;
                address_t evictedWay = myCache.evictWay(addr_acess[ii]);
                way_hit = evictedWay;
                cout << "Evicted way at index: " << evictedWay << endl;
        }
    }
    // Example usage of the cache    
    cout << "Cache state after processing address " << hex << addr_acess[ii] << ":" << " at way index: " << way_hit << endl;
    way_hit = -1; // Reset way_hit for the next iteration
    myCache.printCacheState(); // Print the current state of the cache
}
return 0;
}