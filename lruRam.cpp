#include <iostream>
#include <vector>
using namespace std;

class lruRam {
    public:
        vector<vector<int>> LRU;
        int numSets; // Number of sets in the cache
        int associativity; // Number of ways in the cache
        lruRam(int numSets, int associativity)
            : numSets(numSets), associativity(associativity) {
            LRU.resize(numSets);
            for (int i = 0; i < numSets; ++i) {
                LRU[i].resize(associativity);
                for(int j = 0; j < associativity; ++j) {
                    LRU[i][j] = j; // Initialize LRU indices, way 0 is MRU, way N-1 is LRU having the highest/worst LRU value
                }
            }
        }
        void updateLRU(int setIndex, int wayIndex) {
            if (setIndex < 0 || setIndex >= numSets || wayIndex < 0 || wayIndex >= associativity) {
                throw out_of_range("Invalid set or way index");
            }
            // Move the accessed way to the MRU position
            int lruValue = LRU[setIndex][wayIndex];
            for (int i = 0; i < associativity; ++i) {
                if (LRU[setIndex][i] < lruValue) {
                    LRU[setIndex][i]++;
                }
            }
            LRU[setIndex][wayIndex] = 0; // Set the accessed way to MRU
        }
        int getLRUIndex(int setIndex) {
            if (setIndex < 0 || setIndex >= numSets) {
                throw out_of_range("Invalid set index");
            }
            // Find the way with the highest LRU value (the least recently used)
            int lruWay = 0;
            for (int i = 1; i < associativity; ++i) {
                if (LRU[setIndex][i] > LRU[setIndex][lruWay]) {
                    lruWay = i;
                }
            }
            return lruWay; // Return the index of the least recently used way
        }
        void resetLRU(int setIndex) {
            if (setIndex < 0 || setIndex >= numSets) {
                throw out_of_range("Invalid set index");
            }
            // Reset LRU values for the specified set
            for (int i = 0; i < associativity; ++i) {
                LRU[setIndex][i] = i; // Reset to initial stat
            }
        }
        ~lruRam() {
            // Destructor to clean up if necessary
        }
        void printLRU() {
            for (int i = 0; i < numSets; ++i) {
                cout << "Set " << i << ": ";
                for (int j = 0; j < associativity; ++j) {
                    cout << LRU[i][j] << " ";
                }
                cout << endl;
            }
        }

};