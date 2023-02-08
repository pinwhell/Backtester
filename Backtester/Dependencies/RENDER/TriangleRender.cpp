//
// Created by Pinwhell on 9/5/2022.
//

#include "TriangleRender.h"

TriangleRender::TriangleRender()
{}

TriangleRender::~TriangleRender()
{}

void TriangleRender::setVertexData(const void* data, unsigned int size)
{
    mData = data;
    mSize = size;
}