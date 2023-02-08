#pragma once

#include "IStrategy.h"
#include <memory>
#include "MASeries.h"

class ChartAgent;

class MAStrategy : public IStrategy {
public:

    std::unique_ptr<MASeries> movingAvg1;
    std::unique_ptr<MASeries> movingAvg2;

    MAStrategy(size_t mavgLen1, size_t mavgLen2);

    void Update() override;
    void PrintParameters() override;

    void Render() override;
};