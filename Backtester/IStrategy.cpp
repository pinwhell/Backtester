#include "IStrategy.h"
#include "ChartAgent.h"

IStrategy::IStrategy()
    : mWins(0)
    , mLosses(0)
    , mDrawDown(0)
    , mCumLosses(0)
{
}

float IStrategy::getWinRate()
{
    if ((mWins + mLosses) == 0)
        return 0;

    return ((float)mWins / ((float)mWins + (float)mLosses)) * 100;
}

void IStrategy::setChartAgent(ChartAgent* chartAgent)
{
    mChartAgent = chartAgent;
}

void IStrategy::onLoss()
{
    mLosses++;
    mCumLosses++;
}

void IStrategy::onWin()
{
    mWins++;

    if (mCumLosses > mDrawDown)
    {
        mDrawDown = mCumLosses;
        mCumLosses = 0;
    }
}

float IStrategy::getScore()
{
    return getWinRate()  - mChartAgent->getAvgBarBetweenTrades() * 20; //  
}

void IStrategy::setTpPerc(float perc)
{
    mTpPerc = perc / 100.f;
}

void IStrategy::setSlPerc(float perc)
{
    mSlPerc = perc / 100.f;
}

void IStrategy::PrintResults()
{
    printf("Win Rate %.2f, Wins %d, Losses %d, Drowdown %d, Avg Trade Bars %.2f, Avg Bars Btwn Trades %.2f\n", getWinRate(), mWins, mLosses, mDrawDown, mChartAgent->getAvgTradeBarsDurations(), mChartAgent->getAvgBarBetweenTrades());

}
