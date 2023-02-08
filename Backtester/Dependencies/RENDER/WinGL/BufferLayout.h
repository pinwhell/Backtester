//
// Created by Pinwhell on 9/5/2022.
//

#include<gl/glew.h>
#include<GLFW/glfw3.h>
#include <vector>

#ifndef OPENGLNATIVE_BUFFERLAYOUT_H
#define OPENGLNATIVE_BUFFERLAYOUT_H


struct ElementLayoutDesc{
    unsigned int    glType;
    unsigned int    count;
    bool            normalize;

    static unsigned int SizeFromGlType(unsigned int glType);
};

class BufferLayout{
private:
    std::vector<ElementLayoutDesc> m_ElementsLayout;
    uintptr_t m_Stride;
public:
    BufferLayout();

    unsigned int getStride();
    std::vector<ElementLayoutDesc>& getElements();

    template<typename T>
    void Push(unsigned int nElems)
    {}

    template<>
    void Push<float>(unsigned int nElems)
    {
        m_ElementsLayout.push_back({GL_FLOAT, nElems, false});
        m_Stride += nElems * ElementLayoutDesc::SizeFromGlType(GL_FLOAT);
    }
    template<>
    void Push<int>(unsigned int nElems)
    {
        m_ElementsLayout.push_back({GL_INT, nElems, false});
        m_Stride += nElems * ElementLayoutDesc::SizeFromGlType(GL_INT);
    }

    template<>
    void Push<char>(unsigned int nElems)
    {
        m_ElementsLayout.push_back({GL_UNSIGNED_BYTE, nElems, true});
        m_Stride += nElems * ElementLayoutDesc::SizeFromGlType(GL_UNSIGNED_BYTE);
    }
};

#endif //OPENGLNATIVE_BUFFERLAYOUT_H
