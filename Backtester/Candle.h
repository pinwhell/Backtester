#pragma once

#include <time.h>
#include <deque>
#include <unordered_map>
#include <memory>
#include <cstdint>

enum class CandleType {
    BULLISH,
    BEARISH,
    NEUTRAL
};

struct Candle {
    size_t mId;
    long long mTime;
    time_t mUnixTimeStamp;
    double mOpen;
    double mHigh;
    double mLow;
    double mClose;
    double mVolume;
    struct tm mTm;

    Candle(size_t id, long long time, double open, double high, double low, double close, double volume = 0);

    size_t getSecondsTimeStamp();

    void Print();

    bool isNeutral();
    bool isBullish();
    bool isBearish();

    CandleType getType();
};

#define SEC_TO_MILLIS(x) ((x) * 1000)
#define MIN_TO_MILLIS(x) SEC_TO_MILLIS(x * 60)

struct CandleGroup {
    std::deque<std::unique_ptr<Candle>> mCandlesDeque;
    std::unordered_map<double, Candle*> mCandleUMap;

    void AddCandle(std::unique_ptr<Candle> pCandle);

    std::deque<std::unique_ptr<Candle>>& getCandles();
    Candle* FindCandleByTimeStamp(uint64_t timeStamp, float precision = MIN_TO_MILLIS(2)); // 120000 Millis / 1000 == 120 Sec / 60 == 2 Min
    Candle* getCandleById(size_t id);
    Candle* getForwardCandle(Candle* cand);
    Candle* getPreviousCandle(Candle* cand);
    Candle* getFirstCandle();

    size_t getRenderId(Candle* candle);

    std::vector<Candle*> getCandles(uint64_t from, uint64_t to);
    std::vector<Candle*> getCandlesWideRange(Candle* cand, size_t rangeCnt);
    std::vector<Candle*> getCandlesInverseRange(Candle* cand, size_t rangeCnt);

};