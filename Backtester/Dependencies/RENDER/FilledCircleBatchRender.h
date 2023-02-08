#pragma once
#include "TriangleBatchRender.h"

class FilledCircleBatchRender{
    TriangleBatchRender* m_pTriangleBatch;
    bool m_OwnTriangleBatch = false;
public:
    FilledCircleBatchRender();
    FilledCircleBatchRender(TriangleBatchRender* pTriagleBatch);
    ~FilledCircleBatchRender();

    void Begin();
    void End();
    void DrawCircle(const Vertex& center, float r, int resolution = 4);
    void Render();
};
