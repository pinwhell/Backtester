#include "Render.h"
#include <thread>
#include <GraphUtils.hpp>
#include <Maths.h>

Render* g_MainRender = nullptr;
std::unordered_map<RenderScreenPosition, glm::vec2> renderScreenPositions;
uint32_t gLogTimeStamp = 0;

void Render::UpdateScreenPositions()
{
	float w = getWidth();
	float h = getHeight();

	renderScreenPositions[RSP_CORNER_TOP_LEFT] = glm::vec2(0.f, 0.f);
	renderScreenPositions[RSP_TOP] = glm::vec2(w / 2.f, 0.f);
	renderScreenPositions[RSP_CORNER_TOP_RIGHT] = glm::vec2(w, 0.f);

	renderScreenPositions[RSP_LEFT] = glm::vec2(0.f, h / 2.f);
	renderScreenPositions[RSP_MIDDLE] = glm::vec2(w / 2.f, h / 2.f);
	renderScreenPositions[RSP_RIGHT] = glm::vec2(w, h / 2.f);

	renderScreenPositions[RSP_CORNER_BOTTOM_LEFT] = glm::vec2(0.f, h);
	renderScreenPositions[RSP_BOTTOM] = glm::vec2(w / 2.f, h);
	renderScreenPositions[RSP_CORNER_BOTTOM_REIGHT] = glm::vec2(w, h);
}

Render::Render(FPSTimer * _pFpsTimer)
	: pFpsTimer(_pFpsTimer)
	, bFpsTimerOwner(false)
{
	m_pCamera = new GraphApiCamera();	
	m_pTriangleBatchRender = new TriangleBatchRender();
	m_pFilledCircleBatchRender = new FilledCircleBatchRender(m_pTriangleBatchRender);
	m_pLineBatchRender = new LineBatchRender(m_pTriangleBatchRender);
	m_pCircleBatchRender = new CircleBatchRender(m_pLineBatchRender);
	m_pFilledBoxBatchRender = new FilledBoxBatchRender(m_pTriangleBatchRender);
}

Render::Render(int tFpsASec)
	: pFpsTimer(new FPSTimer(tFpsASec))
	, bFpsTimerOwner(true)
{

	m_pCamera = new GraphApiCamera();
	m_pTriangleBatchRender = new TriangleBatchRender();
	m_pFilledCircleBatchRender = new FilledCircleBatchRender(m_pTriangleBatchRender);
	m_pLineBatchRender = new LineBatchRender(m_pTriangleBatchRender);
	m_pCircleBatchRender = new CircleBatchRender(m_pLineBatchRender);
	m_pFilledBoxBatchRender = new FilledBoxBatchRender(m_pTriangleBatchRender);

}

Render::~Render()
{
	if (bFpsTimerOwner)
		delete pFpsTimer;

	delete m_pTriangleBatchRender;
	delete m_pFilledBoxBatchRender;
	delete m_pFilledCircleBatchRender;
	delete m_pCircleBatchRender;
	delete m_pLineBatchRender;
	delete m_pCamera;
}

std::unordered_map<RenderScreenPosition, glm::vec2>& Render::getRenderScreenPositions()
{
	return renderScreenPositions;
}

void Render::BeginRender()
{
	pFpsTimer->PreFrame();

	/*Start Batching*/
	/*Triangle Based Primitives*/
	m_pFilledCircleBatchRender->Begin();
	m_pFilledBoxBatchRender->Begin();
	m_pTriangleBatchRender->Begin();

	/*Line Based Primitives*/
	m_pCircleBatchRender->Begin();
	m_pLineBatchRender->Begin();
}

void Render::EndRender()
{
	/*End Batching*/
   /*Triangle Based Primitives*/
	m_pFilledCircleBatchRender->End();
	m_pFilledBoxBatchRender->End();
	m_pTriangleBatchRender->End();

	/*Line Based Primitives*/
	m_pCircleBatchRender->End();
	m_pLineBatchRender->End();

	/*Render Batching*/
	/*Line Based Primitives*/
	m_pCircleBatchRender->Render();
	m_pLineBatchRender->Render();

	/*Triangle Based Primitives*/
	m_pFilledCircleBatchRender->Render();
	m_pFilledBoxBatchRender->Render();
	m_pTriangleBatchRender->Render();

	/*FPS Counter Update*/
	pFpsTimer->PostFrame();

	auto currMillis = FPSTimer::GetCurrMillis();

	if (currMillis >= gLogTimeStamp)
	{
		printf("FPS: %d\n", pFpsTimer->getCachedFPS());

		gLogTimeStamp = currMillis + SEC_TO_MILLIS(3);
	}

	if (pFpsTimer->getTargetFPS() != 0)
		pFpsTimer->WaitNextFrame();
}

void Render::DrawFilledCircle(const glm::vec2& p, float radius, const glm::vec4& color) {
	m_pFilledCircleBatchRender->DrawCircle(Vertex(p, GraphUtils::ARGB2RGBA(color)), radius, 40);
}

void Render::DrawCircle(const glm::vec2& p, float radius, const glm::vec4& color,
	float thickness) {

	m_pCircleBatchRender->DrawCircle(Vertex(p, GraphUtils::ARGB2RGBA(color)), radius, thickness, 40);
}

void Render::DrawFilledStrictBox(const glm::vec2& tl, const glm::vec2& tr, const glm::vec2& bl,
	const glm::vec2& br, const glm::vec4& color) {
	m_pFilledBoxBatchRender->DrawFilledBox(
		Vertex(tl, GraphUtils::ARGB2RGBA(color)),
		Vertex(tr, GraphUtils::ARGB2RGBA(color)),
		Vertex(bl, GraphUtils::ARGB2RGBA(color)),
		Vertex(br, GraphUtils::ARGB2RGBA(color))
	);
}

void Render::DrawFilledBox(const glm::vec2& lc, float w, float h, const glm::vec4& color)
{
	glm::vec2 tl, tr, bl, br;
	tl = tr = bl = br = lc;

	tr.x += w;
	bl.y += h;
	br.x += w;
	br.y += h;

	DrawFilledStrictBox(
		tl,
		tr,
		bl,
		br,
		color
	);
}

void Render::DrawLine(const glm::vec2& p1, const glm::vec2& p2, const glm::vec4& color,
	float thickness) {
	m_pLineBatchRender->DrawLine(Vertex(p1, GraphUtils::ARGB2RGBA(color)), Vertex(p2, GraphUtils::ARGB2RGBA(color)), thickness);
}

glm::vec2 a = glm::vec2(0.f, 0.f);

void Render::TestIntersection()
{
	auto w = getWidth();
	auto h = getHeight();

	a.x += pFpsTimer->getDeltaTime() * 12;

	auto b = glm::vec2(w, h);
	auto c = glm::vec2(0.f, h / 2.f);
	auto d = glm::vec2(w, h / 2.f);

	DrawFilledBox(
		glm::vec2(0.f, 0.f),
		w,
		h,
		Colors::cBlack
	);

	DrawLine(
		a,
		b,
		Colors::cCyan
	);

	DrawLine(
		c,
		d,
		Colors::cPurple
	);

	glm::vec2 intersectPoint(0.f);

	if (Maths::IntersectSegments2D(
		a,
		b,
		c,
		d,
		intersectPoint
	))
	{
		DrawCircle(intersectPoint, 30, Colors::cCyan, 2.f);
	}
}

void Render::RenderFPS(const glm::vec2& p)
{
	DrawFormatStr(p, Colors::cCyan, false, "FPS [%d]", pFpsTimer->getCachedFPS());
}

void Render::SetFPSTimer(FPSTimer *_pFPSTimer) {
	pFpsTimer = _pFPSTimer;
}

void Render::PushModel(const glm::mat4& mat)
{
	m_pCamera->PushModel(mat);
	m_pTriangleBatchRender->PushBatch();
}

void Render::PopModel()
{
	m_pTriangleBatchRender->PopBatch(true);
	m_pCamera->PopModel();
}
