#include "Shader.h"


void Shader::InitShaderId()
{
    m_ProgramId = glCreateProgram();
}

void Shader::CompileAttachShader(unsigned int shaderType, const std::string& src)
{
    unsigned int shaderId = CompileShader(shaderType, src);
    glAttachShader(m_ProgramId, shaderId);
}

void Shader::LinkShader()
{
    glLinkProgram(m_ProgramId);
}

Shader::Shader(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc)
        : m_ProgramId(0)
{
    InitShaderId();
    CompileAttachShader(GL_VERTEX_SHADER, vertexShaderSrc);
    CompileAttachShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);
    LinkShader();
}

Shader::~Shader()
{
    Unbind();

    if(m_ProgramId)
        glDeleteProgram(m_ProgramId);
}

void Shader::Bind()
{
    glUseProgram(m_ProgramId);
}

void Shader::Unbind()
{
    glUseProgram(0);
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    // creates an empty shader obj, ready to accept source-code and be compiled
    GLuint hShader = glCreateShader(type);

    // hands the shader source code to the shader object so that it can keep a copy of it
    const char* src = source.c_str();
    glShaderSource(hShader, 1, &src, nullptr);

    // compiles whatever source code is contained in the shader object
    glCompileShader(hShader);

    // Error Handling: Check whether the shader has been compiled
    GLint result;
    glGetShaderiv(hShader, GL_COMPILE_STATUS, &result);	// assigns result with compile operation status
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(hShader, GL_INFO_LOG_LENGTH, &length); // assigns length with length of information log
        char* infoLog = (char*)alloca(length * sizeof(char));	// allocate on stack frame of caller
        glGetShaderInfoLog(hShader, length, &length, infoLog);	// returns the information log for a shader object
        //printf("%s Shader Failed Info: %s"), type == GL_FRAGMENT_SHADER ? STRINGIFY(GL_FRAGMENT_SHADER) : STRINGIFY(GL_VERTEX_SHADER), infoLog);
        glDeleteShader(hShader);
        return static_cast<GLuint>(0);
    }

    return hShader;
}

int Shader::getUniformLocation(const std::string &uniformName) {
    return glGetUniformLocation(m_ProgramId, (const char*)uniformName.c_str());;
}

void Shader::setUniformMat4f(const std::string& uniformName, const glm::mat4 &m4x4) {
    glUniformMatrix4fv(getUniformLocation(uniformName),1, GL_FALSE, (GLfloat *)&m4x4[0]);
}
