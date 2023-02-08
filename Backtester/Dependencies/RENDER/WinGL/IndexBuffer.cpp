//
// Created by Pinwhell on 9/5/2022.
//

#include "IndexBuffer.h"


void IndexBuffer::InitBuffersId()
{
    glGenBuffers(1, &m_Id);
}

void IndexBuffer::UpdateElemCount(unsigned int dataSize, unsigned int elementSize)
{
    m_ElementCount = dataSize / elementSize;
}


IndexBuffer::IndexBuffer()
        : m_ElementCount(0)
        , m_Id(0)
{
    InitBuffersId();
}

IndexBuffer::IndexBuffer(void* data, unsigned int dataSize, unsigned int elementSize, unsigned int bufferUsage)
        : m_ElementCount(0)
        , m_Id(0)
{
    InitBuffersId();
    setData(data, dataSize,elementSize, bufferUsage);
}

IndexBuffer::~IndexBuffer()
{
    Unbind();

    if(m_Id)
        glDeleteBuffers(1, &m_Id);
}

void IndexBuffer::setData(void* data, unsigned int dataSize, unsigned int elementSize, unsigned int bufferUsage)
{
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, bufferUsage);
    UpdateElemCount(dataSize, elementSize);
}

void IndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
}

void IndexBuffer::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int IndexBuffer::getCount() {
    return m_ElementCount;
}

