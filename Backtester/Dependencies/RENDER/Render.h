#pragma once

#include <string>
#include "Colors.h"
#include <unordered_map>
#include "FPSTimer.h"
#include <FilledCircleBatchRender.h>
#include <CircleBatchRender.h>
#include <FilledBoxBatchRender.h>
#include <GraphApiCamera.h>


enum RenderScreenPosition {
	RSP_CORNER_TOP_LEFT,
	RSP_TOP,
	RSP_CORNER_TOP_RIGHT,
	RSP_LEFT,
	RSP_MIDDLE,
	RSP_RIGHT,
	RSP_CORNER_BOTTOM_LEFT,
	RSP_BOTTOM,
	RSP_CORNER_BOTTOM_REIGHT
};

class ISurfaceChangeable {
public:
	virtual void onSurfaceCreated() = 0;
	virtual void onSurfaceChanged(int width, int height) = 0;
};

extern std::unordered_map<RenderScreenPosition,  glm::vec2> renderScreenPositions;

enum class RenderType {
	UNK,
	OPENGLES,
	OPENGL,
	D3D9
};

class Render {
protected:
	FPSTimer* pFpsTimer;
	bool bFpsTimerOwner;

	GraphApiCamera* m_pCamera;
	TriangleBatchRender* m_pTriangleBatchRender;
	FilledCircleBatchRender* m_pFilledCircleBatchRender;
	LineBatchRender* m_pLineBatchRender;
	CircleBatchRender* m_pCircleBatchRender;
	FilledBoxBatchRender* m_pFilledBoxBatchRender;

public:
	Render(FPSTimer* pFpsTimer);
	Render(int tFpsASec);
	~Render();
	std::unordered_map<RenderScreenPosition, glm::vec2>& getRenderScreenPositions();

	virtual void BeginRender();
	virtual void EndRender();
	void DrawLine(const glm::vec2& p1, const glm::vec2& p2, const glm::vec4& color, float thickness = 1.f);
	void DrawFilledBox(const glm::vec2& lc, float w, float h, const glm::vec4& color); // lc mean left corner
	void DrawFilledStrictBox(const glm::vec2& tl, const glm::vec2& tr, const glm::vec2& bl, const glm::vec2& br, const glm::vec4& color);
	void DrawFilledCircle(const glm::vec2& p, float radius, const glm::vec4& color);
	void DrawCircle(const glm::vec2& p, float radius, const glm::vec4& color, float thickness = 1.f);
	virtual void DrawFormatStr(const glm::vec2& p, const glm::vec4& color, bool center, const char* mask, ...) {  }
	virtual void DrawStr(const std::string& str, const glm::vec2& p, const glm::vec4& color, bool center = true) {  }
	virtual void DrawFilledTriangle(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec4& color) { }
	virtual void Reset() {  }
	virtual void Resize(float w, float h, bool reset = true) {  }
	virtual float getWidth() = 0;
	virtual float getHeight() = 0;
	virtual bool getVSync() { return false; }
	virtual RenderType getRenderType() { return RenderType::UNK; }

	void TestIntersection();

	GraphApiCamera* getCamera() { return m_pCamera; };

#if ANDROID_APP
	void RenderFPS(const glm::vec2& p = { 50.f, 50.f });
#else
	void RenderFPS(const glm::vec2& p = { 10.f, 10.f });
#endif
	void UpdateScreenPositions();
	void SetFPSTimer(FPSTimer* pFPSTimer);

	void PushModel(const glm::mat4& mat);
	void PopModel();
} extern *g_MainRender;
