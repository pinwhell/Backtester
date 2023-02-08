#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Render.h>
#include "WGL4TriangleRender.h"

class WGL4Render : public Render, public ISurfaceChangeable
{
    inline static WGL4Render* m_Instance = nullptr;
    float m_Width;
    float m_Height;
    WGL4TriangleRender* m_pWGL4TriangleRender;
public:
    WGL4Render();
    ~WGL4Render();

    static WGL4Render* getInstance();

    virtual float getWidth()  override;
    virtual float getHeight() override;

    void BeginRender();

    void onSurfaceCreated() override;
    void onSurfaceChanged(int width, int height) override;

    RenderType getRenderType() { return RenderType::OPENGLES; }
};

