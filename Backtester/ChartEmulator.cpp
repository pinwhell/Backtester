#define _CRT_SECURE_NO_WARNINGS
#include "ChartEmulator.h"
#include "PairProvider.h"
#include "Candle.h"
#include "IBacktest.h"
#include "Program.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "WGL4Render.h"
#include <Colors.h>

ChartEmulator::ChartEmulator(const std::string& pairName)
    : mPairName(pairName)
{
    if (gPairsProvider.IsPairLoaded(pairName) == false)
        throw "Pair Not Loaded";

    mCandleGroup = gPairsProvider.getCandleGroup(pairName);

    mLastClosedBarId = -1;
    mbChartStarted = false;
    mbChartFinished = false;

    mScaleX = 1.f;
    mScaleY = 1.f;

    setBarPosition(200);
}

const std::string& ChartEmulator::getPairName()
{
    return mPairName;
}

void ChartEmulator::setBarPosition(size_t barId)
{
    if (IsValidBar(barId) == false)
        return;

    mCurrentBarId = barId;
}

size_t ChartEmulator::getBarPosition()
{
    return mCurrentBarId;
}

void ChartEmulator::NotifyAll(ChartEventType evType)
{
    for (IChartEventListener* curr : mEventListeners.mSubscribers)
        curr->onChartEvent(evType);

    mEventListeners.CompleteSubscribe();
    mEventListeners.CompleteUnsubscribe();
}

void ChartEmulator::SubscribeEventListener(IChartEventListener* listener)
{
    listener->setParent(this);
    mEventListeners.Subscribe(listener);
}

void ChartEmulator::UnsuscribeEventListener(IChartEventListener* listener)
{
    listener->setParent(nullptr);
    mEventListeners.Remove(listener);
}

bool ChartEmulator::IsValidBar(size_t barId)
{
    return barId > 0 && barId < mCandleGroup->getCandles().size();
}

bool ChartEmulator::HasNextCandle()
{
    return IsValidBar(mCurrentBarId - 1);
}

void ChartEmulator::Update()
{
    if (mbChartFinished)
        return;

    if (!mbChartStarted)
    {
        NotifyAll(ChartEventType::CHART_START);
        mbChartStarted = true;
    }

    if (mCandleGroup->getCandles().empty())
        return;

    NotifyAll(ChartEventType::CAND_OPEN);

    if (mLastClosedBarId != mCurrentBarId)
    {
        NotifyAll(ChartEventType::CAND_CLOSE);
        mLastClosedBarId = mCurrentBarId;
    }

    if (!HasNextCandle())
    {
        mbChartFinished = true;
        NotifyAll(ChartEventType::CHART_FINISH);
        return;
    }

    mCurrentBarId--;
}

bool ChartEmulator::HasChartFinished()
{
    return mbChartFinished;
}

float ChartEmulator::getCandlePositionX(Candle* cand)
{
    return mCandleGapSize * mCandleGroup->getRenderId(cand);;
}

float ChartEmulator::getCandleClosePositionY(Candle* cand)
{
    glm::vec4 transform = getModelMatrix() * glm::vec4(0.f, cand->mClose, 0.f, 1.f);

    return transform.y - WGL4Render::getInstance()->getHeight() * 0.5f;
}

bool ChartEmulator::setStartDate(const std::string& highTimeDate, const std::string& intraDayDate)
{
    if (highTimeDate.empty())
    {
        mCurrentBarId = mCandleGroup->mCandlesDeque.size() - 1;
        return true;
    }

    struct tm tm {0};

    if (sscanf(highTimeDate.c_str(), "%d/%d/%d", &(tm.tm_mday), &(tm.tm_mon), &(tm.tm_year)) != 3)
    {
        printf("Unable to parse \"%s\" date, make sure it is in dd/mm/yyyy format\n", highTimeDate.c_str());
        return false;
    }

    tm.tm_mon -= 1;
    tm.tm_year -= 1900;

    if (sscanf(intraDayDate.c_str(), "%d:%d:%d", &(tm.tm_hour), &(tm.tm_min), &(tm.tm_sec)) != 3)
    {
        printf("Unable to parse \"%s\" date, make sure it is in hh:mm:ss format\n", intraDayDate.c_str());
        return false;
    }

    uint64_t toFindEpoch = mktime(&tm) * 1000;

    Candle* cand = mCandleGroup->FindCandleByTimeStamp(toFindEpoch);

    if (cand == nullptr)
    {
        printf("Unable to find candles arround %s %s\n", intraDayDate.c_str(), intraDayDate.c_str());
        return false;
    }

    mCurrentBarId = cand->mId;
}

Candle* ChartEmulator::getCurrentBar()
{
    return mCandleGroup->getCandleById(mCurrentBarId);
}

CandleGroup* ChartEmulator::getCandleGroup()
{
    return mCandleGroup.get();
}

void ChartEmulator::Render()
{
    float mPointerX = 0.f;

    mCandleWidth = gWindow->getWindowSize().x * 0.01f;
    mWickWidth = mCandleWidth  * 0.18f;
    mCandleGapSize = gWindow->getWindowSize().x * 0.015f;

    WGL4Render::getInstance()->PushModel(getModelMatrix());

    std::vector<Candle*> candlesInScene = getCandlesInScene();

    for (const auto& curr : candlesInScene)
    {
        size_t currRenderId = mCandleGroup->getRenderId(curr);
        auto currType = curr->getType();
        const glm::vec4* candColor = nullptr;

        switch (currType)
        {
        case CandleType::BULLISH:
            candColor = &Colors::cCyan;
            break;

        case CandleType::NEUTRAL:
            candColor = &Colors::cGray;
            break;

        case CandleType::BEARISH:
            candColor = &Colors::cRed;
            break;
        }

        WGL4Render::getInstance()->DrawFilledBox(glm::vec2(getCandlePositionX(curr) - mCandleWidth * 0.5f, curr->mOpen), (float)mCandleWidth, (float)(curr->mClose - curr->mOpen), *candColor);
        WGL4Render::getInstance()->DrawFilledBox(glm::vec2(getCandlePositionX(curr) - mWickWidth * 0.5f, curr->mHigh), (float)mWickWidth, (float)(curr->mLow - curr->mHigh), *candColor);
    }

    

    WGL4Render::getInstance()->PopModel();
}

std::vector<Candle*> ChartEmulator::getCandlesInScene()
{
    return mCandleGroup->getCandlesInverseRange(getCurrentBar(), 50);
}

glm::mat4 ChartEmulator::getModelMatrix()
{
    return glm::translate(getScaleMatrix(), glm::vec3((WGL4Render::getInstance()->getWidth() * 0.5f), (WGL4Render::getInstance()->getHeight() * 0.5f), 0.f));
}

void ChartEmulator::AddScale(const glm::vec2& addend)
{
    mScaleX += addend.x;

    if (mScaleX < 1.f)
        mScaleX = 1.0f;

    mScaleY += addend.y;

    if (mScaleY < 1.f)
        mScaleY = 1.0f;

    WGL4Render::getInstance()->getCamera()->setPosition(glm::vec2(getCandlePositionX(getCurrentBar()), getCandleClosePositionY(getCurrentBar())) * -1.f);
}

glm::mat4 ChartEmulator::getScaleMatrix()
{
    return glm::scale(glm::mat4(1.f), glm::vec3(mScaleX, mScaleY, 1.f));
}

glm::vec2 ChartEmulator::getCandleClosePosition(Candle* cand)
{
    return glm::vec2(getCandlePositionX(cand), getCandleClosePositionY(cand));
}

void ChartEmulator::MoveCamToCurrent()
{
    WGL4Render::getInstance()->getCamera()->setPosition(getCandleClosePosition(getCurrentBar()) * -1.f);
}

float ChartEmulator::getAvgCandleBodySize()
{
    float avg = 0.f;
    size_t i = 0;

    for (auto curr : getCandlesInScene())
    {
        avg += curr->getBodySize();
        i++;
    }

    if (i != 0)
        avg /= (float)i;

    return avg != 0.f ? avg : 1.f;
}

void IChartEventListener::setParent(ChartEmulator* parent)
{
    mParent = parent;
}

ChartEmulator* IChartEventListener::getParent()
{
    return mParent;
}
