//
// Created by Pinwhell on 12/5/2022.
//

#include "LineBatchRender.h"

LineBatchRender::LineBatchRender()
        : m_pTriangleBatch(new TriangleBatchRender())
        , m_OwnTriangleBatch(true)
{}

LineBatchRender::LineBatchRender(TriangleBatchRender* pTriagleBatch)
        : m_pTriangleBatch(pTriagleBatch)
        , m_OwnTriangleBatch(false)
{}

LineBatchRender::~LineBatchRender()
{
    if(m_OwnTriangleBatch)
        delete m_pTriangleBatch;
}

void LineBatchRender::Begin()
{
    if(m_OwnTriangleBatch)
        m_pTriangleBatch->Begin();
}

void LineBatchRender::End()
{
    if(m_OwnTriangleBatch)
        m_pTriangleBatch->End();
}

glm::vec2 LineBatchRender::Normal(const glm::vec2& origDiff)
{
    glm::vec2 normal;

    normal.x = origDiff.y;
    normal.y = -origDiff.x;

    return glm::normalize(normal);
}

void LineBatchRender::DrawLine(const Vertex& p1, const Vertex& p2, float thickness)
{
    Vertex p1Bot, p1Top;
    Vertex p2Bot, p2Top;

    p1Bot = p1Top = p1;
    p2Bot = p2Top = p2;

    glm::vec3 unitNormalNormalized = glm::vec3(Normal(glm::vec2(p2.pos - p1.pos)) * thickness, 0.f);

    p1Bot.pos -= unitNormalNormalized;
    p1Top.pos += unitNormalNormalized;

    p2Bot.pos -= unitNormalNormalized;
    p2Top.pos += unitNormalNormalized;

    m_pTriangleBatch->DrawTriangle(p1Bot, p2Top, p1Top);
    m_pTriangleBatch->DrawTriangle(p1Bot, p2Bot, p2Top);
}

void LineBatchRender::Render(){
    if(m_OwnTriangleBatch)
        m_pTriangleBatch->Render();
}