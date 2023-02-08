#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <deque>
#include "Candle.h"

class PairProvider {
private:
    std::unordered_map<std::string, std::shared_ptr<CandleGroup>> mPairs;

public:

    bool LoadPairFromFile(const std::string& pairName, const std::string& csvPath);

    static bool LoadTOHLCVFromCSV(const std::string& csvPath, std::shared_ptr<CandleGroup> outCandles);

    std::shared_ptr<CandleGroup> getCandleGroup(const std::string& pair);

    bool IsPairLoaded(const std::string& pairName);

}extern gPairsProvider;
