#include "TriangleBatchRender.h"

void TriangleBatchRender::Begin()
{
    getCurrentBatch()->Reset();
}

void TriangleBatchRender::DrawTriangle(const Vertex& a, const Vertex& b, const Vertex& c)
{
    getCurrentBatch()->Push(a);
    getCurrentBatch()->Push(b);
    getCurrentBatch()->Push(c);
}

void TriangleBatchRender::End()
{
    if(!(getCurrentBatch()->Empty()))
    {
        m_TriangleRender->setVertexData(getCurrentBatch()->getData(), getCurrentBatch()->getSizeInBytes());
    }
}

void TriangleBatchRender::Render()
{
    if (!(getCurrentBatch()->Empty()))
    {
//            LOGI("Drawing %d Triangles", m_BatchVertex->getCount() / 3);
        m_TriangleRender->Render();
    }
}

void TriangleBatchRender::PushBatch()
{
    m_BatchsVertex.push(std::make_unique<Batch<Vertex>>());
}

void TriangleBatchRender::PopBatch(bool bRender)
{
    if (bRender)
    {
        End();
        Render();
    }

    m_BatchsVertex.pop();
}

TriangleBatchRender::TriangleBatchRender()
{
    PushBatch();
}

TriangleBatchRender::~TriangleBatchRender() {
    PopBatch(false);
}

void TriangleBatchRender::setTriangleRender(TriangleRender* triangleRender)
{
    m_TriangleRender = triangleRender;
}

Batch<Vertex>* TriangleBatchRender::getCurrentBatch()
{
    return m_BatchsVertex.top().get();
}
