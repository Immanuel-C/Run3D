#include "Shader.h"

Shader::Shader(const std::string& vertFilePath, const std::string& fragFilePath)
{
    uint32_t vertID;
    uint32_t fragID;
    std::string vertCode = readFile(vertFilePath);
    std::string fragCode = readFile(fragFilePath);

    const char* cVertCode = vertCode.c_str();
    const char* cFragCode = fragCode.c_str();

    int success;
    char infoLog[512];

    vertID = glCreateShader(GL_VERTEX_SHADER);
    fragID = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertID, 1, &cVertCode, nullptr);
    glShaderSource(fragID, 1, &cFragCode, nullptr);

    // Extension: GL_ARB_parallel_shader_compile
    glCompileShader(vertID);
    glGetShaderiv(vertID, GL_COMPLETION_STATUS_ARB, &success);
    if (!success) {
        glGetShaderInfoLog(vertID, 512, NULL, infoLog);
        std::cerr << "Error: Failed to Compile Shader: \n\n" << infoLog;
    }
    
    glCompileShader(fragID);
    glGetShaderiv(fragID, GL_COMPLETION_STATUS_ARB, &success);
    if (!success) {
        glGetShaderInfoLog(fragID, 512, NULL, infoLog);
        std::cerr << "Error: Failed to Compile Shader: \n\n" << infoLog;
    }


    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertID);
    glAttachShader(m_ID, fragID);
    glLinkProgram(m_ID);

    // Extension: GL_ARB_parallel_shader_compile
    glGetProgramiv(m_ID, GL_COMPLETION_STATUS_ARB, &success);
    if (!success) {
        glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
        std::cerr << "Error: Failed to Link Shaders: \n\n" << infoLog;
    }

    glDeleteShader(vertID);
    glDeleteShader(fragID);
}

uint32_t Shader::getID()
{
    return m_ID;
}

void Shader::bind()
{
    glUseProgram(m_ID);
}

void Shader::unbind()
{
    glUseProgram(0);
}

void Shader::destroy()
{
    m_ID = 0;
}
