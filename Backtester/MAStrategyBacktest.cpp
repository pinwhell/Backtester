#include "MAStrategyBacktest.h"

#include "MAStrategy.h"

bool MAStrategyBacktest::Init() {
    for (int i = 1; i < 100; i += 3)
    {
        for (int j = 60; j < 200; j += 3)
        {
            if (i == j)
                continue;

            std::shared_ptr<ChartAgent> chartAgent = std::make_shared<ChartAgent>();
            std::shared_ptr<MAStrategy> strategy = std::make_shared<MAStrategy>(i, j);

            mChart->SubscribeEventListener(chartAgent.get());

            mAgents.push_back(chartAgent);

            chartAgent->setStrategy(strategy);

            strategy->setSlPerc(.06f);
            strategy->setTpPerc(.20f);

        }
    }

    return RegisterAll();
}
