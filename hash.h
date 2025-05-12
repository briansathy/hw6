#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <string>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // w[] array holds the numeric values (base-36 conversions) of each 6-char chunk
        unsigned long long w[5] = {0};

        int n = static_cast<int>(k.size());
        // process up to 5 chunks of 6 chars from the end
        for (int chunk = 0; chunk < 5; ++chunk) {
            int end   = n - chunk * 6;
            if (end <= 0) break;
            int start = end - 6;
            if (start < 0) start = 0;

            unsigned long long value = 0;
            // convert this substring into a base-36 integer
            int idx = start;
            while (idx < end) {
                value = value * 36 + letterDigitToNumber(k[idx]);
                ++idx;
            }
            w[5 - 1 - chunk] = value;
        }

        // combine with rValues
        unsigned long long hashValue = 0;
        for (int i = 0; i < 5; ++i) {
            hashValue += static_cast<unsigned long long>(rValues[i]) * w[i];
        }

        return static_cast<HASH_INDEX_T>(hashValue);

    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // digits
        if (letter >= '0' && letter <= '9') {
            return static_cast<HASH_INDEX_T>(letter - '0' + 26);
        }
        // uppercase letters to lowercase
        if (letter >= 'A' && letter <= 'Z') {
            letter = static_cast<char>(letter - 'A' + 'a');
        }
        // lowercase letters
        if (letter >= 'a' && letter <= 'z') {
            return static_cast<HASH_INDEX_T>(letter - 'a');
        }
        // fallback
        return 0;
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
