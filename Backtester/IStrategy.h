#pragma once

class ChartAgent;

class IStrategy {
public:
    IStrategy();

    size_t mWins;
    size_t mLosses;
    size_t mCumLosses;
    size_t mDrawDown;
    ChartAgent* mChartAgent;

    float mTpPerc;
    float mSlPerc;

    virtual void Update() = 0;
    float getWinRate();
    void setChartAgent(ChartAgent* chartAgent);
    void onLoss();
    void onWin();

    float getScore();

    virtual void PrintParameters() = 0;

    void setTpPerc(float perc);
    void setSlPerc(float perc);

    void PrintResults();

    virtual void Render() {};
};
