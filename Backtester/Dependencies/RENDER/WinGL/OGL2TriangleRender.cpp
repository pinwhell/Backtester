//
// Created by Pinwhell on 9/5/2022.
//

#include "OGL2TriangleRender.h"
#include "Vertex.h"
#include <Windows.h>
#include <gl/GL.h>

void OGL2TriangleRender::Render()
{
    glBegin(GL_TRIANGLES);

    Vertex* pVertexEntry = (Vertex*)mData;

    for(int i = 0; i < mSize / sizeof(Vertex); i++)
    {
        glColor3fv((GLfloat*)&(pVertexEntry[i].color));
        glVertex2fv((GLfloat*)&(pVertexEntry[i].pos));
    }

    glEnd();
}