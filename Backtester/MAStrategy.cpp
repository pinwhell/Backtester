#include "MAStrategy.h"
#include "ChartAgent.h"

MAStrategy::MAStrategy(size_t mavgLen1, size_t mavgLen2)
{
    movingAvg1 = std::make_unique<MASeries>(mavgLen1);
    movingAvg2 = std::make_unique<MASeries>(mavgLen2);
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
            mChartAgent->Long("LongTrade", mTpPerc, mSlPerc);
        else if (MASeries::Crossunder(movingAvg1.get(), movingAvg2.get())) 
            mChartAgent->Short("ShortTrade", mTpPerc, mSlPerc);
    }
}

void MAStrategy::PrintParameters()
{
    printf("[MA Strategy] [ %d %d ]\n", movingAvg1->mLength, movingAvg2->mLength);
}

void MAStrategy::Render()
{
    movingAvg1->Render();
    movingAvg2->Render();
}
