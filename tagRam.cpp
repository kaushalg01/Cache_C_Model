#include "tagElement.cpp"
#include <bits/stdc++.h>

using namespace std;

//This defines the structure of a tagRam

class tagRam {
    public:
        int numSets; // Number of sets in the cache
        int associativity; // Number of ways in the cache
        
        // The tagRam is a 2D vector where each row represents a set and each column represents a way
        // This allows for associativity in the cache design
        vector<vector<tagElement>> tagRam;

        tagRam(int numSets, int associativity)
            : numSets(numSets), associativity(associativity) {
            tagRam.resize(numSets);
            for (int i = 0; i < numSets; ++i) {
                tagRam[i].resize(associativity);
            }
        }
    }