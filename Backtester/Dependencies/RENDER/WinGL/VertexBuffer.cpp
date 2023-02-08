//
// Created by Pinwhell on 9/5/2022.
//

#include "VertexBuffer.h"


void VertexBuffer::InitBuffersId()
{
    glGenBuffers(1, &m_Id);
}

VertexBuffer::VertexBuffer()
    : m_CachedDataSize(0)
{
    InitBuffersId();
}


VertexBuffer::VertexBuffer(void* data, unsigned int dataSize, unsigned int bufferUsage)
{
    InitBuffersId();
    setData(data, dataSize, bufferUsage);
}

VertexBuffer::~VertexBuffer()
{
    Unbind();

    if(m_Id)
        glDeleteBuffers(1, &m_Id);
}

void VertexBuffer::setData(const void* data, unsigned int dataSize, unsigned int bufferUsage)
{
    m_CachedDataSize = dataSize;

    Bind();
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, bufferUsage);
}

void VertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_Id);
}

void VertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned int VertexBuffer::getVertexCount(unsigned int sizeOfVertex) {
    return m_CachedDataSize / sizeOfVertex;
}
