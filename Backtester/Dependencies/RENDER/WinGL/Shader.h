//
// Created by Pinwhell on 9/5/2022.
//

#ifndef OPENGLNATIVE_SHADER_H
#define OPENGLNATIVE_SHADER_H

#include<gl/glew.h>
#include<GLFW/glfw3.h>
#include <string>
#include <glm/matrix.hpp>

class Shader{
private:
    int m_ProgramId;

    void InitShaderId();
    void CompileAttachShader(unsigned int shaderType, const std::string& src);
    void LinkShader();
public:
    Shader(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc);
    ~Shader();

    int getUniformLocation(const std::string& uniformName);
    void setUniformMat4f(const std::string& uniformName, const glm::mat4& m4x4);

    void Bind();
    void Unbind();

    static unsigned int CompileShader(unsigned int type, const std::string& source);
};


#endif //OPENGLNATIVE_SHADER_H
