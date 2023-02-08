//
// Created by Pinwhell on 9/5/2022.
//

#include<gl/glew.h>
#include<GLFW/glfw3.h>

#ifndef OPENGLNATIVE_VERTEXBUFFER_H
#define OPENGLNATIVE_VERTEXBUFFER_H


class VertexBuffer{
private:
    unsigned int m_Id;
    unsigned int m_CachedDataSize;

    void InitBuffersId();
public:
    VertexBuffer();
    VertexBuffer(void* data, unsigned int dataSize, unsigned int bufferUsage = GL_DYNAMIC_DRAW);
    ~VertexBuffer();

    void setData(const void* data, unsigned int dataSize, unsigned int bufferUsage = GL_DYNAMIC_DRAW);
    unsigned int  getVertexCount(unsigned int sizeOfVertex);
    void Bind();
    void Unbind();
};

#endif //OPENGLNATIVE_VERTEXBUFFER_H
