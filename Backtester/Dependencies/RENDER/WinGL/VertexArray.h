//
// Created by Pinwhell on 9/5/2022.
//

#include "BufferLayout.h"
#include "VertexBuffer.h"

#ifndef OPENGLNATIVE_VERTEXARRAY_H
#define OPENGLNATIVE_VERTEXARRAY_H


class VertexArray{
private:
    unsigned int m_Id;

    void InitArrayId();
    void Setup(BufferLayout* bl, VertexBuffer* vb);
public:
    VertexArray(BufferLayout* bl, VertexBuffer* vb);
    ~VertexArray();

    void Bind();
    void Unbind();
};


#endif //OPENGLNATIVE_VERTEXARRAY_H
