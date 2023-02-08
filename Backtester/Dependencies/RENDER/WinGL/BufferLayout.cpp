//
// Created by Pinwhell on 9/5/2022.
//

#include "BufferLayout.h"

unsigned int ElementLayoutDesc::SizeFromGlType(unsigned int glType)
{
    switch (glType) {
        case GL_FLOAT:
        case GL_INT:
            return sizeof(unsigned int);
            break;
        case GL_UNSIGNED_BYTE:
            return sizeof(unsigned char);
            break;
    }

    return 0;
}

BufferLayout::BufferLayout()
        : m_Stride(0)
{}

unsigned int BufferLayout::getStride()
{
    return m_Stride;
}

std::vector <ElementLayoutDesc> &BufferLayout::getElements()
{
    return m_ElementsLayout;
}
