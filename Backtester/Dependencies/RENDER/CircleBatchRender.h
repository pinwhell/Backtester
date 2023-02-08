#pragma once
#include "LineBatchRender.h"

class CircleBatchRender{
    LineBatchRender* m_pLineBatch;
    bool m_OwnLineBatch = false;
public:
    CircleBatchRender();
    CircleBatchRender(LineBatchRender* pLineBatchRender);
    ~CircleBatchRender();

    void Begin();
    void End();
    void Render();
    void DrawCircle(const Vertex& center, float r, float thickness, int resolution = 4);
};
