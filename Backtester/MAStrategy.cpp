#include "MAStrategy.h"
#include "ChartAgent.h"

MAStrategy::MAStrategy(size_t mavgLen1, size_t mavgLen2)
{
    movingAvg1 = std::make_unique<MASeries>(mavgLen1, Colors::cCyan);
    movingAvg2 = std::make_unique<MASeries>(mavgLen2, Colors::cRed);
}

void MAStrategy::Update()
{
    movingAvg1->setParent(mChartAgent->getParent());
    movingAvg2->setParent(mChartAgent->getParent());

    movingAvg1->Update();
    movingAvg2->Update();

    if (mChartAgent->getActiveTradesCount() == 0)
    {
        if (MASeries::Crossover(movingAvg1.get(), movingAvg2.get()))
            onSignal(StrategySignalType::LONG);
        else if (MASeries::Crossunder(movingAvg1.get(), movingAvg2.get())) 
            onSignal(StrategySignalType::SHORT);
    }
}

void MAStrategy::PrintParameters()
{
    printf("[MA Strategy] [ %d %d ]\n", movingAvg1->mLength, movingAvg2->mLength);
}

void MAStrategy::onSignal(StrategySignalType sigType)
{
    switch (sigType)
    {
    case StrategySignalType::LONG:
        mChartAgent->Long("LongTrade", mTpPerc, mSlPerc);
        break;

    case StrategySignalType::SHORT:
        mChartAgent->Short("ShortTrade", mTpPerc, mSlPerc);
        break;
    }
}

void MAStrategy::Render()
{
    movingAvg1->Render();
    movingAvg2->Render();
}

InverseMAStrategy::InverseMAStrategy(size_t mavgLen1, size_t mavgLen2)
    : MAStrategy(mavgLen1, mavgLen2)
{}

void InverseMAStrategy::onSignal(StrategySignalType sigType)
{
    switch (sigType)
    {
    case StrategySignalType::LONG:
        mChartAgent->Short("ShortTrade", mTpPerc, mSlPerc);
        break;

    case StrategySignalType::SHORT:
        mChartAgent->Long("LongTrade", mTpPerc, mSlPerc);
        break;
    }
}
