#include "ChartAgent.h"
#include "ITrade.h"
#include "IStrategy.h"

ChartAgent::ChartAgent()
    : mCurrentStrategy(0x0)
{
    setBalance("USD", 100);
}

void ChartAgent::onChartEvent(ChartEventType evType) {

    const auto& pair = mParent->getPairName();

    switch (evType)
    {

    case ChartEventType::CAND_CLOSE:
        mCurrentStrategy->Update();
        break;


    case ChartEventType::CHART_FINISH:
        //printf("%s Emulation Finished\n", pair.c_str());
        break;

    default:
        break;
    }
}

void ChartAgent::Long(const std::string& tradeId, float TpPerc, float SlPerc)
{
    if (mActiveTrades.find(tradeId) != mActiveTrades.end())
        return;

    std::shared_ptr<ITrade> trade = std::make_shared<LongTrade>();

    float currPrice = mParent->getCurrentBar()->mClose;

    trade->setId(tradeId);
    trade->setPocket(this);
    trade->setStopLoss(currPrice - currPrice * SlPerc);
    trade->setTakeProfit(currPrice + currPrice * TpPerc);

    mParent->SubscribeEventListener(trade.get());

    mTrades.push_back(trade);

    mActiveTrades[tradeId] = trade;

    trade->Open();
}

void ChartAgent::Short(const std::string& tradeId, float TpPerc, float SlPerc)
{
    if (mActiveTrades.find(tradeId) != mActiveTrades.end())
        return;

    std::shared_ptr<ITrade> trade = std::make_shared<ShortTrade>();

    float currPrice = mParent->getCurrentBar()->mClose;

    trade->setId(tradeId);
    trade->setPocket(this);
    trade->setStopLoss(currPrice + currPrice * SlPerc);
    trade->setTakeProfit(currPrice - currPrice * TpPerc);

    mParent->SubscribeEventListener(trade.get());

    mTrades.push_back(trade);

    mActiveTrades[tradeId] = trade;

    trade->Open();
}

void ChartAgent::setStrategy(std::shared_ptr<IStrategy> strategy)
{
    mCurrentStrategy = strategy;
    mCurrentStrategy->setChartAgent(this);

}

std::shared_ptr<IStrategy> ChartAgent::getStrategy()
{
    return mCurrentStrategy;
}

void ChartAgent::onTradeExit(ITrade* pTrade, bool win)
{
    IPocket::onTradeExit(pTrade, win);

    if (mCurrentStrategy)
    {
        if (win)
            mCurrentStrategy->onWin();
        else
            mCurrentStrategy->onLoss();
    }
}

float ChartAgent::getScore()
{
    return mCurrentStrategy->getScore();
}

void ChartAgent::PrintTrades()
{
    for (auto currTrade : mTrades)
        currTrade->PrintResults();
}

float ChartAgent::getAvgTradeBarsDurations()
{
    float avg = 0.f;
    size_t tradeCnt = 0;

    for (auto& curr : mTrades)
    {
        tradeCnt++;
        avg += curr->getBarDuration();
    }

    if (tradeCnt > 0)
        avg /= tradeCnt;

    return avg;
}

float ChartAgent::getAvgBarBetweenTrades()
{
    float lastTradeOpen = 0.f;
    float avg = 0.f;
    size_t tradeCnt = 0;

    for (auto& curr : mTrades)
    {
        auto currCandEntry = curr->getEntryCandle();

        if (!currCandEntry)
            continue;

        tradeCnt++;
        avg += currCandEntry->mClose - lastTradeOpen;

        lastTradeOpen = currCandEntry->mClose;
    }

    if (tradeCnt > 0)
        avg /= tradeCnt;

    return avg;
}

void ChartAgent::Render()
{
    RenderTrades();

    if (mCurrentStrategy)
        mCurrentStrategy->Render();
}
