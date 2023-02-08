//
// Created by Pinwhell on 9/5/2022.
//
#pragma once
#include <Render.h>
#include "OGL2TriangleRender.h"

//#define TESTING

class OGL2Render : public Render, public ISurfaceChangeable {
private:

    inline static OGL2Render* m_Instance = nullptr;
    float m_Width;
    float m_Height;
    OGL2TriangleRender* m_pOGL2TriangleRender;
public:
    OGL2Render();
    ~OGL2Render();

    static OGL2Render* getInstance();

    virtual float getWidth()  override;
    virtual float getHeight() override;

    void onSurfaceCreated() override;
    void onSurfaceChanged(int width, int height) override;

    RenderType getRenderType() { return RenderType::OPENGL; }
    void Resize(float w, float h, bool reset = true) override;
};
