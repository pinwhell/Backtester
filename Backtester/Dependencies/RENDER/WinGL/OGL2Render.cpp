#include "OGL2Render.h"
#include <Maths.h>
#include <Windows.h>
#include <GLFW/glfw3.h>
#include <GraphUtils.hpp>

OGL2Render::OGL2Render()
        : m_Width(0.f)
        , m_Height(0.f)
        , Render(new FPSTimer(0))
{
    
    m_pOGL2TriangleRender = new OGL2TriangleRender();

    m_pTriangleBatchRender->setTriangleRender(m_pOGL2TriangleRender);
}

OGL2Render::~OGL2Render() {
    delete m_pOGL2TriangleRender;
}

OGL2Render* OGL2Render::getInstance() {
    if(!m_Instance)
        m_Instance = new OGL2Render();

    return m_Instance;
}

void OGL2Render::onSurfaceCreated()
{
}

void OGL2Render::onSurfaceChanged(int width, int height)
{
    m_Width = float(width);
    m_Height = float(height);

    UpdateScreenPositions();

    m_pCamera->Update(m_Width, m_Height);

    glViewport(0, 0, width, height);
}

/*void OGL2Render::BeginRender()
{
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

unsigned int gLogTimeStip = FPSTimer::GetCurrMillis();

void OGL2Render::EndRender()
{}*/

void OGL2Render::Resize(float w, float h, bool reset)
{
    onSurfaceChanged(int(w),int(h));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(m_pCamera->getLeft(), m_pCamera->getRight(), m_pCamera->getBottom(), m_pCamera->getTop(), -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

float OGL2Render::getWidth() {
    return m_Width;
}

float OGL2Render::getHeight() {
    return m_Height;
}

