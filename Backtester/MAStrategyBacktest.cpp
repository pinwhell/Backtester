#include "MAStrategyBacktest.h"

#include "MAStrategy.h"

bool MAStrategyBacktest::Init() {
    for (int i = 1; i < 40; i++)
    {
        for (int j = 80; j < 150; j++)
        {
            if (i == j)
                continue;

            std::shared_ptr<ChartAgent> chartAgent = std::make_shared<ChartAgent>();
            std::shared_ptr<MAStrategy> strategy = std::make_shared<MAStrategy>(i, j);

            mChart->SubscribeEventListener(chartAgent.get());

            mAgents.push_back(chartAgent);

            chartAgent->setStrategy(strategy);

            strategy->setSlPerc(.06f);
            strategy->setTpPerc(.06f);

        }
    }

    return RegisterAll();
}

bool InverseMAStrategyBacktest::Init()
{
    for (int i = 1; i < 40; i++)
    {
        for (int j = 80; j < 150; j++)
        {
            if (i == j)
                continue;

            std::shared_ptr<ChartAgent> chartAgent = std::make_shared<ChartAgent>();
            std::shared_ptr<InverseMAStrategy> strategy = std::make_shared<InverseMAStrategy>(i, j);

            mChart->SubscribeEventListener(chartAgent.get());

            mAgents.push_back(chartAgent);

            chartAgent->setStrategy(strategy);

            strategy->setSlPerc(.06f);
            strategy->setTpPerc(.06f);

        }
    }

    return RegisterAll();
}
