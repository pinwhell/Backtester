#include "WGL4Render.h"

WGL4Render::WGL4Render()
	: m_Width(0.f)
	, m_Height(0.f)
	, Render(new FPSTimer(0))
{
	m_pWGL4TriangleRender = new WGL4TriangleRender();
	m_pWGL4TriangleRender->setCamera(m_pCamera);
	m_pTriangleBatchRender->setTriangleRender(m_pWGL4TriangleRender);
}

WGL4Render::~WGL4Render()
{
	delete m_pWGL4TriangleRender;
}

WGL4Render* WGL4Render::getInstance()
{
	if (!m_Instance)
		m_Instance = new WGL4Render();

	return m_Instance;
}

float WGL4Render::getWidth()
{
	return m_Width;
}

float WGL4Render::getHeight()
{
	return m_Height;
}

void WGL4Render::BeginRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Render::BeginRender();
}

void WGL4Render::onSurfaceCreated()
{
}

void WGL4Render::onSurfaceChanged(int width, int height)
{
	m_Width = float(width);
	m_Height = float(height);

	UpdateScreenPositions();

	m_pCamera->Update(m_Width, m_Height);

	glViewport(0, 0, width, height);
}
