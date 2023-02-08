#include "ITrade.h"
#include "IPocket.h"
#include "Program.h"
#include "WGL4Render.h"

ITrade::ITrade()
{

}

void ITrade::setId(const std::string& id)
{
    mId = id;
}

const std::string& ITrade::getId()
{
    return mId;
}

void ITrade::setStopLoss(double sl)
{
    mSLPrice = sl;
}

void ITrade::setTakeProfit(double tp)
{
    mTPPrice = tp;
}

void ITrade::onChartEvent(ChartEventType evType) {

    switch (evType)
    {

    case ChartEventType::CAND_CLOSE:
        Update();
        break;

    default:
        break;
    }
}

void ITrade::onTargetHit(bool win)
{
    mExitCandle = mParent->getCurrentBar();

    mbWinner = win;

    if (win)
        mExit = mTPPrice;
    else
        mExit = mSLPrice;

    mParent->UnsuscribeEventListener(this);
    mpPocket->onTradeExit(this, win);
}

void ITrade::onSlHit()
{
    //printf("Sl Hit: "); mParent->getCurrentBar()->Print();
}

void ITrade::onTpHit()
{
    //printf("Tp Hit: "); mParent->getCurrentBar()->Print();
}

void ITrade::Update()
{
    mLossDelta = mEntry - mSLPrice;
    mWinDelta = mEntry - mTPPrice;
}

void ITrade::Open()
{
    //printf("Open Trade at: "); mParent->getCurrentBar()->Print();
    mEntryCandle = mParent->getCurrentBar();
    mEntry = mEntryCandle->mClose;
}

void ITrade::setPocket(IPocket* pPocket)
{
    mpPocket = pPocket;
}

// [ 2020-xx-xx ] O[xxxxx.xx] C[xxxx.xx] R[Losser]
// [ 2020-xx-xx ] O[xxxxx.xx] C[xxxx.xx] R[Winner]

#include <ctime>

void ITrade::PrintResults()
{
    if (mEntryCandle)
    {
        const auto tm = mEntryCandle->mTm;

        printf(" [ %d/%d/%d %d:%d ]", tm.tm_mday, tm.tm_mon, tm.tm_year + 1900, tm.tm_hour, tm.tm_min);
    }

    if (mEntry != 0)
    {
        printf("\tO[ %.2f ]", mEntry);

        if (mExit != 0)
        {
            printf(" C[ %.2f ]", mExit);
        }
    }

    if (mbWinner)
        printf(" R[Winner]");
    else 
        printf(" R[Losser]");

    printf("\n");
}

size_t ITrade::getBarDuration()
{
    if (!(mEntryCandle && mExitCandle))
        return 0;

    return std::abs((int)mEntryCandle->mId - (int)mExitCandle->mId);
}

void ITrade::Render()
{
    if (!mEntryCandle)
        return;

    float startTradePosX = gChart->getCandlePositionX(mEntryCandle);
    float endTradePosX = 0.f; 

    if (mExitCandle)
        endTradePosX = gChart->getCandlePositionX(mExitCandle);
    else
        endTradePosX = gChart->getCandlePositionX(gChart->getCurrentBar());

    WGL4Render::getInstance()->DrawFilledBox({ startTradePosX , mEntryCandle->mClose }, endTradePosX - startTradePosX, -mLossDelta, Colors::Alpha(Colors::cRed, 0.3f));
    WGL4Render::getInstance()->DrawFilledBox({ startTradePosX , mEntryCandle->mClose }, endTradePosX - startTradePosX, -mWinDelta, Colors::Alpha(Colors::cCyan, 0.3f));
}

Candle* ITrade::getEntryCandle()
{
    return mEntryCandle;
}

void LongTrade::Update() {

    ITrade::Update();

    const Candle* currCand = mParent->getCurrentBar();

    bool bStopHit = currCand->mLow < mSLPrice;
    bool bTPHit = currCand->mHigh > mTPPrice;

    if (bStopHit)
        onSlHit();
    else if (bTPHit)
        onTpHit();

    if (bStopHit || bTPHit)
        onTargetHit(bTPHit);

    return;
}

void ShortTrade::Update()
{
    ITrade::Update();

    const Candle* currCand = mParent->getCurrentBar();

    bool bStopHit = currCand->mHigh > mSLPrice;
    bool bTPHit = currCand->mLow < mTPPrice;

    if (bStopHit)
        onSlHit();
    else if (bTPHit)
        onTpHit();

    if (bStopHit || bTPHit)
        onTargetHit(bTPHit);

    return;
}
