#pragma once

#include "TriangleBatchRender.h"

class FilledBoxBatchRender{
    TriangleBatchRender* m_pTriangleBatch;
    bool m_OwnTriangleBatch = false;
public:
    FilledBoxBatchRender();
    FilledBoxBatchRender(TriangleBatchRender* pTriagleBatch);
    ~FilledBoxBatchRender();
    void Begin();
    void End();
    void DrawFilledBox(const Vertex& tl, const Vertex& tr, const Vertex& bl, const Vertex& br);
    void Render();
};