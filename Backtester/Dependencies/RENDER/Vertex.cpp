//
// Created by Pinwhell on 9/5/2022.
//

#include "Vertex.h"

Vertex::Vertex()
    : Vertex(glm::vec2(0.f, 0.f))
{}

Vertex::Vertex(const glm::vec2& _pos)
        : Vertex(glm::vec3(_pos.x, _pos.y, 0.f), glm::vec4(0.f, 0.f, 0.f, 1.f))
{}

Vertex::Vertex(const glm::vec2& _pos, const glm::vec4& _color)
        : Vertex(glm::vec3(_pos.x, _pos.y, 0.f), _color)
{}

Vertex::Vertex(const glm::vec3& _pos, const glm::vec4& _color)
{
    pos = _pos;
    color = _color;
}