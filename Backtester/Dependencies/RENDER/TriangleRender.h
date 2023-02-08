//
// Created by Pinwhell on 9/5/2022.
//

#pragma once

#include <cstdint>

class TriangleRender{
protected:
    const void* mData;
    size_t mSize;

public:

    TriangleRender();
    ~TriangleRender();

    virtual void setVertexData(const void* data, unsigned int size);
    virtual void Render() = 0;
};
