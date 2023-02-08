#include "PairProvider.h"
#include "ChartEmulator.h"
#include "ChartAgent.h"
#include "MAStrategy.h"
#include "IBacktest.h"
#include <Window.h>
#include <Windows.h>
#include "WGL4Render.h"
#include <glm/gtc/matrix_transform.hpp>

class MAStrategyBacktest : public IBackTest
{
public:
    bool Init() override {
        for (int i = 1; i < 100; i+=3)
        {
            for (int j = 60; j < 200; j +=3)
            {
                if (i == j)
                    continue;

                std::shared_ptr<ChartAgent> chartAgent = std::make_shared<ChartAgent>();
                std::shared_ptr<MAStrategy> strategy = std::make_shared<MAStrategy>(i, j);

                mChart->SubscribeEventListener(chartAgent.get());

                mAgents.push_back(chartAgent);

                chartAgent->setStrategy(strategy);

                strategy->setSlPerc(.06f);
                strategy->setTpPerc(.20f);

            }
        }

        return RegisterAll();
    }
};

#include <functional>
#include "Program.h"

std::function<void(const glm::vec2&)> gOnMouseDragCallback;
std::function<void(GLFWwindow* window, const glm::vec2&)> gMouseMoveCallback;
std::function<void(GLFWwindow* window, double xoffset, double yoffset)> gScrollCallbackFrwd;

std::shared_ptr<ap::Window> gWindow;
ChartEmulator* gChart;

int main()
{
    if (gPairsProvider.LoadPairFromFile("BTCUSDT", "BTCUSDT.csv") == false)
        return 1;

    std::unique_ptr<ChartEmulator> chartEmu = std::make_unique<ChartEmulator>("BTCUSDT");
    std::unique_ptr<MAStrategyBacktest> maBacktest = std::make_unique<MAStrategyBacktest>();

    gChart = chartEmu.get();

     maBacktest->setChart(chartEmu.get());

    if (!maBacktest->Init())
        return 2;

    //maBacktest->PrintResult();

    if (chartEmu->setStartDate() == false)
        return 3;

    gWindow = std::make_shared<ap::Window>(1280, 720, "Window");

    WGL4Render::getInstance()->onSurfaceChanged(1280, 720);

    WGL4Render::getInstance()->PushModel(glm::translate(glm::mat4(1.f), glm::vec3((WGL4Render::getInstance()->getWidth() * 0.5f), (WGL4Render::getInstance()->getHeight() * 0.5f), 0.f)));

    WGL4Render::getInstance()->getCamera()->setPosition({0.f , -chartEmu->getCurrentBar()->mClose});


#define ACUM_STEP_SIZE 1.f
    
    glm::vec2 stepStart(0.f);
    bool bLastMouseClicked = false;

    gScrollCallbackFrwd = [&](GLFWwindow* window, double xoffset, double yoffset)
    {
        if (gWindow->getMousePosition().x > gWindow->getWindowSize().x * 0.8f)
            chartEmu->AddScale({ 0.f, yoffset / 16.f });

        if (gWindow->getMousePosition().y > gWindow->getWindowSize().y * 0.8f)
            chartEmu->AddScale({ yoffset / 16.f, 0.f });

    };

    glfwSetScrollCallback(gWindow->glfw_ptr(), [](GLFWwindow* window, double xoffset, double yoffset) {
        if (gScrollCallbackFrwd)
            gScrollCallbackFrwd(window, xoffset, yoffset);
        });

    gOnMouseDragCallback = [&](const glm::vec2& deltaMove) {

        if (gWindow->getMousePosition().x > gWindow->getWindowSize().x * 0.8f)
        {
            chartEmu->AddScale({ 0.f, (deltaMove.y) / 175.f });
            return;
        }

        if (gWindow->getMousePosition().y > gWindow->getWindowSize().y * 0.9f)
        {
            chartEmu->AddScale({ deltaMove.x / 300.f , 0.f });
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

    float widthScaleX = 1.f;
    float widthScaleY = 1.f;

    float acumUpdate = 0.f;    

    ShowCursor(false);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    while (gWindow->isOpen())
    {
        WGL4Render::getInstance()->BeginRender();

        glm::vec2 mousePosWld;
        glm::vec2 mousePos = gWindow->getMousePosition();

        WGL4Render::getInstance()->getCamera()->ScreenToWorldPoint(mousePos, mousePosWld);

        WGL4Render::getInstance()->DrawFilledCircle(mousePosWld, 5.f, Colors::cCyan);

        WGL4Render::getInstance()->DrawFilledCircle(mousePosWld, 15.f, Colors::cCyan + glm::vec4(0.f, 0.f, 0.f, -0.8f));

        //float dtime = clock.GetElapsedTimeAsMilliseconds();


        if (chartEmu->HasChartFinished() == false && GetAsyncKeyState(VK_SPACE))
        {
            WGL4Render::getInstance()->getCamera()->setPosition(chartEmu->getCandleClosePosition(chartEmu->getCurrentBar()) * -1.f);
            chartEmu->Update();
        }

        chartEmu->Render();
        maBacktest->Render();

        WGL4Render::getInstance()->EndRender();
        gWindow->onUpdate();
        //clock.Restart();
    }

    maBacktest->getBestAgent()->getStrategy()->PrintParameters();
    maBacktest->PrintResult();
}