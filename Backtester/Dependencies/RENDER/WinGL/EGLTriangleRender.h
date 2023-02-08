//
// Created by Pinwhell on 9/5/2022.
//

#pragma once


#include "TriangleRender.h"
#include <GraphApiCamera.h>
#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include <string>

class EGLTriangleRender : public TriangleRender {
private:
    const std::string m_VertexShaderSrc = std::string("#version 300 es\n "
                                                                "layout(location = 0) in vec4 vPos;"
                                                                "layout(location = 1) in vec4 vColor;"
                                                                "uniform mat4 vProj;"
                                                                "out vec4 fColor;"
                                                                "void main(){ "
                                                                "gl_Position = vProj * vPos;"
                                                                "fColor = vColor;"
                                                                "}");

    const std::string m_FragShaderSrc = std::string( "#version 300 es\n"
                                                              "precision highp float;"
                                                              "out vec4 oColor;"
                                                              "in vec4 fColor;"
                                                              "void main(){"
                                                              "oColor = fColor;"
                                                              "}");

protected:
    Shader* m_Shader;
    VertexBuffer* m_Vb;
    VertexArray* m_Va;
    GraphApiCamera* m_pCamera;

public:
    EGLTriangleRender();
    ~EGLTriangleRender();

    void setCamera(GraphApiCamera* camera) { m_pCamera = camera; }
    void setVertexData(const void* data, unsigned int size) override;
    void Render() override;
};
