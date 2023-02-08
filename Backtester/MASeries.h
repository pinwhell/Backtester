#pragma once

#include "ISeries.h"
#include <Colors.h>

class MASeries : public ISeries {
private:
    glm::vec4 mColor;

public:
    MASeries(size_t length, const glm::vec4& color = Colors::cCyan);
    MASeries(size_t length);

    size_t mLength;

    void Update() override;

    static bool Crossover(MASeries* a, MASeries* b);

    static bool Crossunder(MASeries* a, MASeries* b);

    void setLength(size_t length);

    void Render() override;

    void setColor(const glm::vec4& color);
};