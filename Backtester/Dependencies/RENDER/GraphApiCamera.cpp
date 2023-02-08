#include "GraphApiCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

const glm::mat4 &GraphApiCamera::getProj() {
    return m_Proj;
}

const glm::mat4& GraphApiCamera::getModel()
{
    return m_ModelStack.top();
}

glm::mat4 GraphApiCamera::getMVP()
{
    return m_Proj * m_View * m_ModelStack.top();
}

glm::vec2 GraphApiCamera::getPosition()
{
    return m_Translation;
}

void GraphApiCamera::setPosition(const glm::vec2& pos)
{
    m_Translation = glm::vec3(pos, 0.f);

    m_View = glm::translate(glm::mat4(1.f), m_Translation);
}

void GraphApiCamera::Update(float width, float height) {
    m_Width = width;
    m_Height = height;

    UpdateProj();
}

void GraphApiCamera::UpdateProj() {
    m_Top = m_Height;
    m_Bottom = 0.f;
    m_Left = 0.f;
    m_Right = m_Width;

    m_Proj = glm::ortho(m_Left, m_Right, m_Bottom,m_Top , -1.f, 1.f);
//    m_Proj = glm::ortho(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);
}

void GraphApiCamera::PushModel(const glm::mat4& mat)
{

    m_ModelStack.push(mat);

}

void GraphApiCamera::PopModel()
{

    m_ModelStack.pop();

}


float GraphApiCamera::getLeft() {
    return m_Left;
}

float GraphApiCamera::getRight() {
    return m_Right;
}

float GraphApiCamera::getTop() {
    return m_Top;
}

float GraphApiCamera::getBottom() {
    return m_Bottom;
}

void GraphApiCamera::Translate(const glm::vec2& translation)
{
    m_Translation += glm::vec3(translation, 0.f);

    m_View = glm::translate(glm::mat4(1.f), m_Translation);
}

void GraphApiCamera::ScreenToWorldPoint(const glm::vec2& screenPoint, glm::vec2& worldPoint)
{
    double normalizedX = -1.0 + 2.0 * (double)screenPoint.x / m_Width;
    double normalizedY = 1.0 - 2.0 * (double)screenPoint.y / m_Height;
    auto NDC = glm::inverse(getMVP()) * glm::vec4(normalizedX, normalizedY, 0.f, 1.f);
    worldPoint = glm::vec2(NDC.x / NDC.w, NDC.y / NDC.w);
}
