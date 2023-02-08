//
// Created by Pinwhell on 11/5/2022.
//

#include "FilledCircleBatchRender.h"

FilledCircleBatchRender::FilledCircleBatchRender()
        : m_pTriangleBatch(new TriangleBatchRender())
        , m_OwnTriangleBatch(true)
{}

FilledCircleBatchRender::FilledCircleBatchRender(TriangleBatchRender* pTriagleBatch)
    : m_pTriangleBatch(pTriagleBatch)
    , m_OwnTriangleBatch(false)
{}

FilledCircleBatchRender::~FilledCircleBatchRender()
{
    if(m_OwnTriangleBatch)
        delete m_pTriangleBatch;
}

void FilledCircleBatchRender::Begin()
{
    if(m_OwnTriangleBatch)
        m_pTriangleBatch->Begin();
}

void FilledCircleBatchRender::End()
{
    if(m_OwnTriangleBatch)
        m_pTriangleBatch->End();
}

void FilledCircleBatchRender::DrawCircle(const Vertex& center, float r, int resolution)
{
    float stepDeltaAng = 360.f / float(resolution);

    Vertex zeroVert = Vertex(glm::vec2(center.pos.x + cosf(0.f) * r, center.pos.y + sinf(0.f) * r), center.color);
    Vertex prevVert = zeroVert;

    for(float currStepDeltaAng = stepDeltaAng; currStepDeltaAng < 360.f; currStepDeltaAng += stepDeltaAng)
    {
        Vertex currVert = Vertex(glm::vec2(center.pos.x + cosf(glm::radians(currStepDeltaAng)) * r, center.pos.y + sinf(glm::radians(currStepDeltaAng)) * r), center.color);

        m_pTriangleBatch->DrawTriangle(center, currVert, prevVert);

        prevVert = currVert;
    }

    m_pTriangleBatch->DrawTriangle(center,prevVert, zeroVert);
}

void FilledCircleBatchRender::Render()
{
    if(m_OwnTriangleBatch)
        m_pTriangleBatch->Render();
}