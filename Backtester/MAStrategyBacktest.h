#pragma once

#include "IBacktest.h"

class MAStrategyBacktest : public IBackTest
{
public:
    bool Init() override;
};

class InverseMAStrategyBacktest : public IBackTest
{
public:
    bool Init() override;
};

