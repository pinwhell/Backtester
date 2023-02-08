#include "WGL4TriangleRender.h"
#include <Vertex.h>

WGL4TriangleRender::WGL4TriangleRender()
{
    BufferLayout vertexLayout;

    vertexLayout.Push<float>(3); // Position
    vertexLayout.Push<float>(4); // Color

    m_Shader = new Shader(m_VertexShaderSrc, m_FragShaderSrc);
    m_Vb = new VertexBuffer();
    m_Va = new VertexArray(&vertexLayout, m_Vb);
}

WGL4TriangleRender::~WGL4TriangleRender()
{
    delete m_Vb;
    delete m_Va;
    delete m_Shader;
}

void WGL4TriangleRender::setVertexData(const void* data, unsigned int size)
{
    m_Vb->setData(data, size, GL_DYNAMIC_DRAW);
}

void WGL4TriangleRender::Render()
{
    m_Va->Bind();
    m_Shader->Bind();
    m_Shader->setUniformMat4f("vMVP", m_pCamera->getMVP());

    glDrawArrays(GL_TRIANGLES, 0, m_Vb->getVertexCount(sizeof(Vertex)));
}
