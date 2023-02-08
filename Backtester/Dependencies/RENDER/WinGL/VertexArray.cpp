#include "VertexArray.h"

void VertexArray::InitArrayId()
{
    glGenVertexArrays(1, &m_Id);
}

void VertexArray::Setup(BufferLayout* bl, VertexBuffer* vb)
{
    Bind();
    vb->Bind();

    auto& elements = bl->getElements();
    unsigned int currELemOffset = 0;

    for(int i = 0; i < elements.size(); i++)
    {
        auto& currElement = elements[i];

        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, currElement.count, currElement.glType,currElement.normalize ? GL_TRUE : GL_FALSE, bl->getStride(), (void*)currELemOffset );
        currELemOffset += currElement.count * ElementLayoutDesc::SizeFromGlType(currElement.glType);
    }
    Unbind();
}

VertexArray::VertexArray(BufferLayout* bl, VertexBuffer* vb)
{
    InitArrayId();
    Setup(bl, vb);
}

VertexArray::~VertexArray()
{
    Unbind();

    if(m_Id)
        glDeleteVertexArrays(1, &m_Id);
}

void VertexArray::Bind()
{
    glBindVertexArray(m_Id);
}

void VertexArray::Unbind()
{
    glBindVertexArray(0);
}
