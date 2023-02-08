//
// Created by Pinwhell on 12/5/2022.
//

#include <glm/matrix.hpp>
#include <stack>

#ifndef LAUNCHER_CAMERA_H
#define LAUNCHER_CAMERA_H


class GraphApiCamera {
private:
    glm::vec3 m_Translation;
    glm::mat4 m_MVP;
    glm::mat4 m_View;
    glm::mat4 m_Proj;
    float m_Width;
    float m_Height;
    float m_Right;
    float m_Left;
    float m_Bottom;
    float m_Top;

    std::stack<glm::mat4> m_ModelStack;

    void UpdateProj();
public:

    void PushModel(const glm::mat4& mat);
    void PopModel();

    GraphApiCamera()
        : m_Width(0.f)
        , m_Height(0.f)
        , m_Right(0.f)
        , m_Left(0.f)
        , m_Bottom(0.f)
        , m_Top(0.f)
        , m_Proj(glm::mat4(1.f))
        , m_View(glm::mat4(1.f))
        , m_Translation(0.f)
    {
        m_ModelStack.push(glm::mat4(1.f));
    }

    void setPosition(const glm::vec2& pos);

    GraphApiCamera(float width, float height)
    {
        Update(width, height);
    }

    GraphApiCamera(int width, int height)
    {
        Update(float(width), float(height));
    }

    void Update(float width, float height);
    const glm::mat4& getProj();
    const glm::mat4& getModel();
    glm::mat4 getMVP();

    glm::vec2 getPosition();


    float getLeft();
    float getRight();
    float getTop();
    float getBottom();
    void Translate(const glm::vec2& translation);
    void ScreenToWorldPoint(const glm::vec2& screenPoint, glm::vec2& worldPoint);
};


#endif //LAUNCHER_CAMERA_H
