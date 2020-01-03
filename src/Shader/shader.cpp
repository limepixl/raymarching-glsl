#include "shader.hpp"
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <ctime>

// Load whole file as a string
std::string loadFromFile(const char* shaderPath)
{
    std::ifstream shaderRaw(shaderPath);

    std::stringstream shaderStream;
    shaderStream << shaderRaw.rdbuf();

    std::string shaderString = shaderStream.str();
    return shaderString;
}

// Load shaders and create a shader program
void Shader::LoadShader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexString = loadFromFile(vertexPath);
    std::string fragmentString = loadFromFile(fragmentPath);

    const char* vertexSource = vertexString.c_str();
    const char* fragmentSource = fragmentString.c_str();

    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSource, nullptr);
    glCompileShader(vertex);

    // Check compilation status
    int compiled;
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &compiled);
    if (compiled != GL_TRUE)
    {
        int log_length = 0;
        char message[1024];
        glGetShaderInfoLog(vertex, 1024, &log_length, message);
        printf("%s\n", message);
    }

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSource, nullptr);
    glCompileShader(fragment);

    // Check compilation status
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &compiled);
    if (compiled != GL_TRUE)
    {
        int log_length = 0;
        char message[1024];
        glGetShaderInfoLog(fragment, 1024, &log_length, message);
        printf("%s\n", message);
    }

    // Program creation and linking
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    // Clean up
    glDetachShader(ID, vertex);
    glDetachShader(ID, fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::UseShader()
{
    glUseProgram(ID);
}