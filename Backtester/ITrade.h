#pragma once

#include "ChartEmulator.h"

class IPocket;

class ITrade : public IChartEventListener {
protected:
    std::string mId;

    IPocket* mpPocket;

    Candle* mEntryCandle;
    Candle* mExitCandle;
    double mEntry;  // Price Where Trade Open
    double mExit;   // Price Where Trade Exit
    double mSLPrice;
    double mTPPrice;
    bool mbWinner;

    double mLossDelta;
    double mWinDelta;

public:

    ITrade();

    void setId(const std::string& id);

    const std::string& getId();

    void setStopLoss(double sl);

    void setTakeProfit(double tp);

    void onChartEvent(ChartEventType evType) override;

    void onTargetHit(bool win);

    void onSlHit();

    void onTpHit();

    virtual void Update();

    virtual void Open();

    void setPocket(IPocket* pPocket);

    void PrintResults();

    size_t getBarDuration();

    /* Expected to be in Price Model Matrix Context*/
    void Render();

    Candle* getEntryCandle();
};

class LongTrade : public ITrade {
public:

    // When a candle close this is called

    void Update() override;
};

class ShortTrade : public ITrade {
public:

    // When a candle close this is called

    void Update() override;
};
