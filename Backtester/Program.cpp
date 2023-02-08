#include "Program.h"

#include <functional>
#include "WGL4Render.h"
#include "PairProvider.h"
#include <glm/gtc/matrix_transform.hpp>
#include <Windows.h>
#include "MAStrategyBacktest.h"
#include "IStrategy.h"
#include <thread>


std::function<void(const glm::vec2&)> gOnMouseDragCallback;
std::function<void(GLFWwindow* window, const glm::vec2&)> gMouseMoveCallback;
std::function<void(GLFWwindow* window, double xoffset, double yoffset)> gScrollCallbackFrwd;

std::unique_ptr<ap::Window> gWindow;
std::unique_ptr<ChartEmulator> gBtcChart;
ChartEmulator* gChart;

#define ACUM_STEP_SIZE 1.f

glm::vec2 stepStart(0.f);
bool bLastMouseClicked = false;

bool Program::Init()
{
    if (gPairsProvider.LoadPairFromFile("BTCUSDT", "BTCUSDT.csv") == false)
        return false;

    gBtcChart = std::make_unique<ChartEmulator>("BTCUSDT");

    gChart = gBtcChart.get();

    //maBacktest->PrintResult();

    if (gChart->setStartDate() == false)
        return false;

    gWindow = std::make_unique<ap::Window>(1280, 720, "Window");

    WGL4Render::getInstance()->onSurfaceChanged(1280, 720);

    WGL4Render::getInstance()->PushModel(glm::translate(glm::mat4(1.f), glm::vec3((WGL4Render::getInstance()->getWidth() * 0.5f), (WGL4Render::getInstance()->getHeight() * 0.5f), 0.f)));

    WGL4Render::getInstance()->getCamera()->setPosition({ 0.f , -gChart->getCurrentBar()->mClose });


    gScrollCallbackFrwd = [&](GLFWwindow* window, double xoffset, double yoffset)
    {
        if (gWindow->getMousePosition().x > gWindow->getWindowSize().x * 0.8f)
            gChart->AddScale({ 0.f, yoffset / 16.f });

        if (gWindow->getMousePosition().y > gWindow->getWindowSize().y * 0.8f)
            gChart->AddScale({ yoffset / 16.f, 0.f });

    };

    glfwSetScrollCallback(gWindow->glfw_ptr(), [](GLFWwindow* window, double xoffset, double yoffset) {
        if (gScrollCallbackFrwd)
        gScrollCallbackFrwd(window, xoffset, yoffset);
        });

    gOnMouseDragCallback = [&](const glm::vec2& deltaMove) {

        if (gWindow->getMousePosition().x > gWindow->getWindowSize().x * 0.8f)
        {
            gChart->AddScale({ 0.f, (deltaMove.y) / 175.f });
            return;
        }

        if (gWindow->getMousePosition().y > gWindow->getWindowSize().y * 0.9f)
        {
            gChart->AddScale({ deltaMove.x / 300.f , 0.f });
            return;
        }

        WGL4Render::getInstance()->getCamera()->Translate(deltaMove);
    };

    gMouseMoveCallback = [&](GLFWwindow* window, const glm::vec2& mousePos) {
        bool bMouseCLicked = gWindow->isMouseButtonPressed(ap::MOUSE_BUTTON_1);

        if (bMouseCLicked != bLastMouseClicked)
        {
            if (bMouseCLicked)
                stepStart = mousePos;

            bLastMouseClicked = bMouseCLicked;
        }

        if (bMouseCLicked)
        {
            auto currMouse = mousePos;
            auto dMouseStep = currMouse - stepStart;

            if (glm::distance(glm::vec2(0.f), dMouseStep) >= ACUM_STEP_SIZE)
            {
                if (gOnMouseDragCallback)
                    gOnMouseDragCallback(dMouseStep);
                stepStart = currMouse;
            }
        }
    };

    glfwSetCursorPosCallback(gWindow->glfw_ptr(), [](GLFWwindow* window, double xpos, double ypos) {
        gWindow->cursor_position_callback(window, xpos, ypos);

    if (gMouseMoveCallback)
        gMouseMoveCallback(window, glm::vec2(xpos, -ypos));
        });

    ShowCursor(false);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    return true;
}

void Program::Run()
{
    std::unique_ptr<MAStrategyBacktest> maBacktest = std::make_unique<MAStrategyBacktest>();

    maBacktest->setChart(gChart);

    if (!maBacktest->Init())
        return;

    while (gWindow->isOpen())
    {
        WGL4Render::getInstance()->BeginRender();

        // Drawing Pointer
        glm::vec2 mousePosWld;
        glm::vec2 mousePos = gWindow->getMousePosition();

        WGL4Render::getInstance()->getCamera()->ScreenToWorldPoint(mousePos, mousePosWld);
        WGL4Render::getInstance()->DrawFilledCircle(mousePosWld, 5.f, Colors::cCyan);
        WGL4Render::getInstance()->DrawFilledCircle(mousePosWld, 15.f, Colors::cCyan + glm::vec4(0.f, 0.f, 0.f, -0.8f));


        // Updating Chart
        if (gChart->HasChartFinished() == false && GetAsyncKeyState(VK_SPACE))
        {
            gChart->MoveCamToCurrent();
            gChart->Update();
        }

        // Drawing Chart
        gChart->Render();
        maBacktest->Render();

        WGL4Render::getInstance()->EndRender();
        gWindow->onUpdate();
    }

    maBacktest->getBestAgent()->getStrategy()->PrintParameters();
    maBacktest->PrintResult();
}
