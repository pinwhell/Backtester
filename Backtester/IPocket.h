#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

class ITrade;

class IPocket {
private:
    std::unordered_map<std::string, float> mBalanceBook;

protected:
    std::vector<std::shared_ptr<ITrade>> mTrades;
    std::unordered_map<std::string, std::shared_ptr<ITrade>> mActiveTrades;
public:

    void setBalance(const std::string& currency, float ammount);

    float getBalance(const std::string& currency);

    virtual void onTradeExit(ITrade* pTrade, bool win);

    size_t getActiveTradesCount();

    void RenderTrades();
};