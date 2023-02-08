#pragma once
#include "TriangleBatchRender.h"

class LineBatchRender{
    TriangleBatchRender* m_pTriangleBatch;
    bool m_OwnTriangleBatch = false;
public:
    LineBatchRender();
    LineBatchRender(TriangleBatchRender* pTriagleBatch);
    ~LineBatchRender();

    void Begin();
    void End();
    void DrawLine(const Vertex& p1, const Vertex& p2, float thickness);
    void Render();

    static glm::vec2 Normal(const glm::vec2& origDiff);
};