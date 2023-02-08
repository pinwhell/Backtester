#define _CRT_SECURE_NO_WARNINGS

#include "Candle.h"
#include <cstdio>

Candle::Candle(size_t id, long long time, double open, double high, double low, double close, double volume)
    : mId(id)
    , mTime(time)
    , mOpen(open)
    , mHigh(high)
    , mLow(low)
    , mClose(close)
    , mVolume(volume)
    , mUnixTimeStamp(mTime / 1000)
{
    mTm = *localtime(&mUnixTimeStamp);
}

size_t Candle::getSecondsTimeStamp()
{
    return mUnixTimeStamp;
}

void Candle::Print()
{
    printf("[ %d ] [ %d/%d/%d ] [ %d:%d ]: %t O[%f] H[%f] L[%f] C[%f] V[%f]\n", mId, mTm.tm_mday, mTm.tm_mon + 1, 1900 + mTm.tm_year, mTm.tm_hour, mTm.tm_min, mOpen, mHigh, mLow, mClose, mVolume);
}

bool Candle::isNeutral()
{
    return getType() == CandleType::NEUTRAL;
}

bool Candle::isBullish()
{
    return getType() == CandleType::BULLISH;
}

bool Candle::isBearish()
{
    return getType() == CandleType::BEARISH;
}

CandleType Candle::getType()
{
    if (mClose > mOpen)
        return CandleType::BULLISH;

    if (mClose < mOpen)
        return CandleType::BEARISH;

    return CandleType::NEUTRAL;
}

void CandleGroup::AddCandle(std::unique_ptr<Candle> pCandle)
{
    mCandleUMap[pCandle->mTime] = pCandle.get();
    mCandlesDeque.push_front(std::move(pCandle));
}

std::deque<std::unique_ptr<Candle>>& CandleGroup::getCandles()
{
    return mCandlesDeque;
}

Candle* CandleGroup::FindCandleByTimeStamp(uint64_t timeStamp, float precision)
{
    if (mCandleUMap.find(timeStamp) != mCandleUMap.end())
        return mCandleUMap[timeStamp];

    auto result = std::find_if(mCandleUMap.begin(), mCandleUMap.end(), [&](const auto& kv) {
        return std::abs((double)kv.second->mTime - timeStamp) < precision;
        });
    
    if (result == mCandleUMap.end())
        return nullptr;

    return (*result).second;
}

Candle* CandleGroup::getCandleById(size_t id)
{
    if (id < 0 || id > mCandlesDeque.size() - 1)
        return nullptr;

    return mCandlesDeque[id].get();
}

Candle* CandleGroup::getForwardCandle(Candle* cand)
{
    return getCandleById(cand->mId - 1);
}

Candle* CandleGroup::getPreviousCandle(Candle* cand)
{
    return getCandleById(cand->mId + 1);
}

Candle* CandleGroup::getFirstCandle()
{
    if (mCandlesDeque.empty())
        return nullptr;

    return mCandlesDeque[mCandlesDeque.size() - 1].get();
}

size_t CandleGroup::getRenderId(Candle* candle)
{
    Candle* firstCandle = getFirstCandle();

    if (firstCandle == nullptr)
        return (size_t)-1;

    if(firstCandle->mTime > candle->mTime)
        return (size_t)-1;

    return std::abs(firstCandle->mUnixTimeStamp - candle->mUnixTimeStamp) / 60;
}

std::vector<Candle*> CandleGroup::getCandles(uint64_t from, uint64_t to)
{
    std::vector<Candle*> candles;

    Candle* firstCand = getFirstCandle();

    if (firstCand == nullptr)
        return candles;

    Candle* startFrom = FindCandleByTimeStamp(from);

    if (!startFrom)
    {
        // 2 scenarios, first is that we got a timestamp after all the candles
        // second scenarios is that we got a time stamp prevois all the candles

        if (from > firstCand->mTime)
            return candles;

        startFrom = firstCand;
    }

    Candle* endat = FindCandleByTimeStamp(to);

    if (!endat)
        return candles;

    if(startFrom->mId > endat->mId + 1)
        return candles;

    for (int i = startFrom->mId; i < endat->mId + 1; i++)
        candles.push_back(mCandlesDeque[i].get());

    return candles;
}

std::vector<Candle*> CandleGroup::getCandlesWideRange(Candle* cand, size_t rangeCnt)
{
    std::vector<Candle*> candRange;

    for (int i = 1; i < rangeCnt && cand->mId + i < mCandlesDeque.size(); i++)
        candRange.push_back(mCandlesDeque[cand->mId + i].get());
    
    for (int i = 0; i < rangeCnt && cand->mId - i >= 0; i++)
        candRange.push_back(mCandlesDeque[cand->mId - i].get());


    return candRange;
}

std::vector<Candle*> CandleGroup::getCandlesInverseRange(Candle* cand, size_t rangeCnt)
{
    std::vector<Candle*> candRange;

    for (int i = 0; i < rangeCnt && cand->mId + i < mCandlesDeque.size(); i++)
        candRange.push_back(mCandlesDeque[cand->mId + i].get());

    return candRange;
}
