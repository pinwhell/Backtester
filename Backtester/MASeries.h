#pragma once

#include "ISeries.h"

class MASeries : public ISeries {
public:
    MASeries(size_t length);

    size_t mLength;

    void Update() override;

    static bool Crossover(MASeries* a, MASeries* b);

    static bool Crossunder(MASeries* a, MASeries* b);

    void setLength(size_t length);

    void Render() override;
};