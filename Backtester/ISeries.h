#pragma once

#include <unordered_map>
#include <vector>
#include "ChartEmulator.h"

class ISeries {
public:
    ChartEmulator* mParent;
    std::unordered_map<Candle*, double> mSerieUMap;
    std::deque<std::pair<Candle*, double>> mSortedSerie;
    std::deque<double> mSerie;

    virtual void Update() = 0;
    void setParent(ChartEmulator* chartEmu);

    virtual void Render() {}
};