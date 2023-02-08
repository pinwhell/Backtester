#include "MASeries.h"
#include "WGL4Render.h"

MASeries::MASeries(size_t length, const glm::vec4& color)
{
    setLength(length);
    setColor(color);
}

MASeries::MASeries(size_t length)
    : MASeries(length, Colors::cCyan)
{}

void MASeries::Update() {
    float currAvg = 0.f;
    Candle* entryCandle = mParent->getCurrentBar();;
    Candle* pCurrCandle = entryCandle;

    if (mSerieUMap.find(pCurrCandle) != mSerieUMap.end())
        return;

    CandleGroup* pCandGroup = mParent->getCandleGroup();
    size_t candCnt = 0;

    if (pCurrCandle)
    {
        do {

            currAvg += pCurrCandle->mClose;
            candCnt++;

        } while ((pCurrCandle = pCandGroup->getPreviousCandle(pCurrCandle)) && candCnt < mLength);

        currAvg /= candCnt;
    }

    mSerie.push_front(currAvg);
    mSerieUMap[entryCandle] = currAvg;
    mSortedSerie.push_front(*mSerieUMap.find(entryCandle));
}

bool MASeries::Crossover(MASeries* a, MASeries* b)
{
    if ((a->mSerie.size() > 1 && b->mSerie.size() > 1) == false)
        return false;

    return a->mSerie[0] > b->mSerie[0] && a->mSerie[1] < b->mSerie[1];
}

bool MASeries::Crossunder(MASeries* a, MASeries* b)
{
    if ((a->mSerie.size() > 1 && b->mSerie.size() > 1) == false)
        return false;

    return a->mSerie[0] < b->mSerie[0] && a->mSerie[1] > b->mSerie[1];
}

void MASeries::setLength(size_t length)
{
    mLength = length;
}

void MASeries::Render()
{
    if ((mSortedSerie.size() > 1) == false)
        return;

    WGL4Render::getInstance()->PushModel(mParent->getModelMatrix());

    for (int i = 1; i < mSortedSerie.size(); i++)
    {
        if (i - 1 > INVERSE_CANDLE_COUNT)
            break;

        WGL4Render::getInstance()->DrawLine({ mParent->getCandlePositionX(mSortedSerie[i - 1].first), mSortedSerie[i - 1].second }, { mParent->getCandlePositionX(mSortedSerie[i].first), mSortedSerie[i].second}, mColor,mParent->getAvgCandleBodySize() * 0.1f);
    }

    WGL4Render::getInstance()->PopModel();
}

void MASeries::setColor(const glm::vec4& color)
{
    mColor = color;
}
