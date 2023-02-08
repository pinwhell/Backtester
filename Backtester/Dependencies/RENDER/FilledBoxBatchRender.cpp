//
// Created by Pinwhell on 17/5/2022.
//

#include "FilledBoxBatchRender.h"

FilledBoxBatchRender::FilledBoxBatchRender()
        : m_pTriangleBatch(new TriangleBatchRender())
        , m_OwnTriangleBatch(true)
{}

FilledBoxBatchRender::FilledBoxBatchRender(TriangleBatchRender* pTriagleBatch)
        : m_pTriangleBatch(pTriagleBatch)
        , m_OwnTriangleBatch(false)
{}

FilledBoxBatchRender::~FilledBoxBatchRender()
{
    if(m_OwnTriangleBatch)
        delete m_pTriangleBatch;
}

void FilledBoxBatchRender::Begin()
{
    if(m_OwnTriangleBatch)
        m_pTriangleBatch->Begin();
}

void FilledBoxBatchRender::End()
{
    if(m_OwnTriangleBatch)
        m_pTriangleBatch->End();
}

void FilledBoxBatchRender::DrawFilledBox(const Vertex& tl, const Vertex& tr, const Vertex& bl, const Vertex& br)
{
    m_pTriangleBatch->DrawTriangle(tr, tl, bl);
    m_pTriangleBatch->DrawTriangle(tr, bl, br);
}

void FilledBoxBatchRender::Render()
{
    if(m_OwnTriangleBatch)
        m_pTriangleBatch->Render();
}
