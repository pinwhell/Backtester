#include <glm/glm.hpp>

#ifndef LAUNCHER_VERTEX_H
#define LAUNCHER_VERTEX_H


struct Vertex{
    glm::vec3 pos;
    glm::vec4 color;

    Vertex();
    Vertex(const glm::vec2& _pos);
    Vertex(const glm::vec2& _pos, const glm::vec4& _color);
    Vertex(const glm::vec3& _pos, const glm::vec4& _color);
};


#endif //LAUNCHER_VERTEX_H
