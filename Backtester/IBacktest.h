#pragma once

#include <vector>
#include <memory>
#include "ChartAgent.h"

class ChartEmulator;

class IBackTest {
protected:
    std::vector<std::shared_ptr<ChartAgent>> mAgents;
    ChartEmulator* mChart;

public:

    virtual bool Init() = 0;

    virtual void PrintResult();

    std::shared_ptr<ChartAgent> getBestAgent();

    bool RegisterAll();

    void setChart(ChartEmulator* chart);

    virtual void Render();
};
