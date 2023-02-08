//
// Created by Pinwhell on 14/5/2022.
//

#include "CircleBatchRender.h"

CircleBatchRender::CircleBatchRender()
        : m_pLineBatch(new LineBatchRender())
        , m_OwnLineBatch(true)
{}

CircleBatchRender::CircleBatchRender(LineBatchRender* pLineBatchRender)
        : m_pLineBatch(pLineBatchRender)
        , m_OwnLineBatch(false)
{}

CircleBatchRender::~CircleBatchRender()
{
    if(m_OwnLineBatch)
        delete m_pLineBatch;
}

void CircleBatchRender::Begin()
{
    if(m_OwnLineBatch)
        m_pLineBatch->Begin();
}

void CircleBatchRender::End()
{
    if(m_OwnLineBatch)
        m_pLineBatch->End();
}

void CircleBatchRender::DrawCircle(const Vertex& center, float r, float thickness, int resolution)
{
    float stepDeltaAng = 360.f / float(resolution);

    Vertex zeroVert = Vertex(glm::vec2(center.pos.x + cosf(0.f) * r, center.pos.y + sinf(0.f) * r), center.color);
    Vertex prevVert = zeroVert;

    for(float currStepDeltaAng = stepDeltaAng; currStepDeltaAng < 360.3f; currStepDeltaAng += stepDeltaAng)
    {
        Vertex currVert = Vertex(glm::vec2(center.pos.x + cosf(glm::radians(currStepDeltaAng)) * r,
                                           center.pos.y + sinf(glm::radians(currStepDeltaAng)) * r),
                                 center.color);

        m_pLineBatch->DrawLine(currVert, prevVert, thickness);

        prevVert = currVert;
    }

    m_pLineBatch->DrawLine(prevVert, zeroVert, thickness);
}

void CircleBatchRender::Render()
{
    if(m_OwnLineBatch)
        m_pLineBatch->Render();
}