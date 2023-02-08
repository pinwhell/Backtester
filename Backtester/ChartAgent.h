#pragma once

#include "ChartEmulator.h"
#include "IPocket.h"
#include <memory>

class IStrategy;

class ChartAgent : public IChartEventListener, public IPocket {

private:
    std::shared_ptr<IStrategy> mCurrentStrategy;

public:

    ChartAgent();

    void onChartEvent(ChartEventType evType) override;

    void Long(const std::string& tradeId, float TpPerc, float SlPerc);
    void Short(const std::string& tradeId, float TpPerc, float SlPerc);

    void setStrategy(std::shared_ptr<IStrategy> strategy);
    std::shared_ptr<IStrategy> getStrategy();

    void onTradeExit(ITrade* pTrade, bool win) override;

    float getScore();

    void PrintTrades();

    float getAvgTradeBarsDurations();

    void Render();

    float getAvgBarBetweenTrades();
};