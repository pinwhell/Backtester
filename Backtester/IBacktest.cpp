#include "IBacktest.h"
#include "ChartAgent.h"
#include "IStrategy.h"

void IBackTest::PrintResult() {
    std::shared_ptr<ChartAgent> bestAgent = getBestAgent();
    std::shared_ptr<IStrategy> bestStrategy = bestAgent->getStrategy();

    bestStrategy->PrintResults();
    bestAgent->PrintTrades();
}

std::shared_ptr<ChartAgent> IBackTest::getBestAgent()
{
    std::shared_ptr<ChartAgent> bestAgent;

    float bestScore = -9999999.f;

    for (auto& currAgent : mAgents)
    {
        float currScore = currAgent->getScore();

        if (currScore > bestScore)
        {
            bestScore = currScore;
            bestAgent = currAgent;
        }
    }

    return bestAgent;
}

bool IBackTest::RegisterAll()
{
    if (mChart == nullptr)
        return false;

    for (auto& currAgent : mAgents)
        mChart->SubscribeEventListener(currAgent.get());

    return true;
}

void IBackTest::setChart(ChartEmulator* chart)
{
    mChart = chart;
}

void IBackTest::Render()
{
    auto bestAgent = getBestAgent();

    bestAgent->Render();
}
