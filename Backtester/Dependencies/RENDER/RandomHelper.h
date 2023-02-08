#pragma once

#include <cstdint>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string>

class RandomHelper{
    public:
    static inline void InitRandomization()
    {
        srand(uint32_t(time(NULL)));
    }

    static inline uint32_t getRandomKey()
    {
        return rand();
    }

    static inline uint32_t getRandomInBtw(int from, int to)
    {
        int d = abs(to - from);
        return from + (getRandomKey() % d);
    }

    static inline float getRandomInBtw(float from, float to)
    {
        float perc = float(rand()) / float(RAND_MAX);
        float d = abs(to - from);
        
        return from + (d * perc);
    }

    static inline std::string getRandomString(size_t len)
    {
        std::string abcdary = std::string((const char*)"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ012345678");
        size_t abcdaryLen = abcdary.size();
        std::string result = "";

        for(size_t i = 0; i < len; i++)
            result.push_back(abcdary[getRandomInBtw(int(0), int(abcdaryLen - 1))]);

        return result;
    }
};