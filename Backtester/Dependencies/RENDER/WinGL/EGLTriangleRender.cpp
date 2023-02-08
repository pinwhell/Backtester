//
// Created by Pinwhell on 9/5/2022.
//

#include "EGLTriangleRender.h"
#include "Vertex.h"

EGLTriangleRender::EGLTriangleRender()
{
    BufferLayout vertexLayout;

    vertexLayout.Push<float>(3); // Position
    vertexLayout.Push<float>(4); // Color

    m_Shader = new Shader(m_VertexShaderSrc, m_FragShaderSrc);
    m_Vb = new VertexBuffer();
    m_Va = new VertexArray(&vertexLayout, m_Vb);
}

EGLTriangleRender::~EGLTriangleRender()
{
    delete m_Vb;
    delete m_Va;
    delete m_Shader;
}

void EGLTriangleRender::setVertexData(const void* data, unsigned int size)
{
    m_Vb->setData(data, size, GL_DYNAMIC_DRAW);
}

void EGLTriangleRender::Render() 
{
    m_Va->Bind();
    m_Shader->Bind();
    m_Shader->setUniformMat4f("vProj", m_pCamera->getProj());

    glDrawArrays(GL_TRIANGLES, 0, m_Vb->getVertexCount(sizeof(Vertex)));
}