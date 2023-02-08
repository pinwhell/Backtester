#include "IPocket.h"
#include "ITrade.h"
#include "Program.h"
#include "WGL4Render.h"

void IPocket::setBalance(const std::string& currency, float ammount)
{
    mBalanceBook[currency] = ammount;
}

float IPocket::getBalance(const std::string& currency)
{
    if (mBalanceBook.find(currency) == mBalanceBook.end())
        setBalance(currency, 0.f);

    return mBalanceBook[currency];
}

void IPocket::onTradeExit(ITrade* pTrade, bool win)
{
    mActiveTrades.erase(pTrade->getId());
}

size_t IPocket::getActiveTradesCount()
{
    size_t acTrades = 0;

    for (const auto& kv : mActiveTrades)
        acTrades++;

    return acTrades;
}

void IPocket::RenderTrades()
{
    WGL4Render::getInstance()->PushModel(gChart->getModelMatrix());

    for (auto& currTrade : mTrades)
    {
        currTrade->Render();
    }

    WGL4Render::getInstance()->PopModel();
}
