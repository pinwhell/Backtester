//
// Created by Pinwhell on 9/5/2022.
//

#pragma once

#include "TriangleRender.h"
#include <Vertex.h>
#include <Batch.h>
#include <stack>
#include <memory>

class TriangleBatchRender {

private:
    std::stack<std::unique_ptr<Batch<Vertex>>> m_BatchsVertex;
    TriangleRender* m_TriangleRender;
public:
    TriangleBatchRender();
    ~TriangleBatchRender();

    void Begin();
    void DrawTriangle(const Vertex& a, const Vertex& b, const Vertex& c);
    void End();
    void Render();

    void PushBatch();
    void PopBatch(bool bRender = false);

    void setTriangleRender(TriangleRender* triangleRender);

    Batch<Vertex>* getCurrentBatch();
};
