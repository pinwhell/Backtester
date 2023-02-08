#pragma once

#include "IChart.h"
#include "Listeners.h"
#include "Candle.h"
#include <string>
#include <memory>
#include <deque>
#include <glm/glm.hpp>

class IChartEventListener;
class IBackTest;

class ChartEmulator {

private:
    std::string mPairName;
    std::shared_ptr<CandleGroup> mCandleGroup;
    Listeners<IChartEventListener*> mEventListeners;
    size_t mCurrentBarId;
    size_t mLastOpenBarId;
    size_t mLastClosedBarId;
    bool mbChartStarted;
    bool mbChartFinished;
    float mCandleGapSize;
    float mCandleWidth;
    float mWickWidth;

    float mScaleX;
    float mScaleY;

public:
    ChartEmulator(const std::string& pairName);

    const std::string& getPairName();

    void setBarPosition(size_t barId);

    size_t getBarPosition();

    void NotifyAll(ChartEventType evType);

    void SubscribeEventListener(IChartEventListener* listener);

    void UnsuscribeEventListener(IChartEventListener* listener);

    bool IsValidBar(size_t barId);

    bool HasNextCandle();

    void Update();

    bool HasChartFinished();

    float getCandlePositionX(Candle* cand);
    float getCandleClosePositionY(Candle* cand);

    // for the HightimeDate is in format dd/mm/yyyy
    // for the intraDayDate is in format hh:mm:ss
    bool setStartDate(const std::string& highTimeDate = std::string(""), const std::string& intraDayDate = std::string("00:00:00"));

    Candle* getCurrentBar();

    CandleGroup* getCandleGroup();

    void Render();

    std::vector<Candle*> getCandlesInScene();

    glm::mat4 getModelMatrix();

    void AddScale(const glm::vec2& addend);

    glm::mat4 getScaleMatrix();

    glm::vec2 getCandleClosePosition(Candle* cand);

    void MoveCamToCurrent();

    float getAvgCandleBodySize();
};

class IChartEventListener {
protected:
    ChartEmulator* mParent;

public:
    virtual void onChartEvent(ChartEventType evType) = 0;

    void setParent(ChartEmulator* parent);
    ChartEmulator* getParent();
};
