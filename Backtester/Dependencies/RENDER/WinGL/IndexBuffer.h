//
// Created by Pinwhell on 9/5/2022.
//

#include<gl/glew.h>
#include<GLFW/glfw3.h>

#ifndef OPENGLNATIVE_INDEXBUFFER_H
#define OPENGLNATIVE_INDEXBUFFER_H


class IndexBuffer{
private:
    unsigned int m_Id;
    unsigned int m_ElementCount;

    void InitBuffersId();

    void UpdateElemCount(unsigned int dataSize, unsigned int elementSize);
public:
    IndexBuffer();
    IndexBuffer(void* data, unsigned int dataSize, unsigned int elementSize, unsigned int bufferUsage = GL_DYNAMIC_DRAW);
    ~IndexBuffer();

    void setData(void* data, unsigned int dataSize, unsigned int elementSize, unsigned int bufferUsage = GL_DYNAMIC_DRAW);
    void Bind();
    void Unbind();
    unsigned int getCount();
};

#endif //OPENGLNATIVE_INDEXBUFFER_H
