#include "shader.hpp"
#include <cstdio>
#include <cstdlib>
#include <Windows.h>
#include <glad/glad.h>
#include <ctime>

// Load whole file as a string
char* loadFromFile(const char* shaderPath)
{
    size_t size;
    char* buffer = NULL;

    FILE* vRaw = fopen(shaderPath, "rb");
    if(vRaw == nullptr)
        printf("Failed to open shader file! Path: %s\n", shaderPath);
    else
    {
        fseek(vRaw, 0L, SEEK_END);
        size = (size_t)ftell(vRaw);
        rewind(vRaw);

        buffer = (char*)malloc(size + 1);   // +1 for \0
        buffer[size] = '\0';

        fread(buffer, 1, size, vRaw);
        fclose(vRaw);
    }

    return buffer;
}

// Load shaders and create a shader program
void Shader::LoadShader(const char* vertexPath, const char* fragmentPath)
{
    char* vertexSource = loadFromFile(vertexPath);
    char* fragmentSource = loadFromFile(fragmentPath);

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
    free(vertexSource);
    free(fragmentSource);
}

void Shader::UseShader()
{
    glUseProgram(ID);
}

// Windows implementation of the function below
bool Shader::CheckChanged(const wchar_t* vertexPath, const wchar_t* fragmentPath)
{
    WIN32_FILE_ATTRIBUTE_DATA vertexData, fragmentData;
    GetFileAttributesEx(vertexPath, GetFileExInfoStandard, &vertexData);
    GetFileAttributesEx(fragmentPath, GetFileExInfoStandard, &fragmentData);

    FILETIME vertexTime = vertexData.ftLastWriteTime;
    FILETIME fragmentTime = fragmentData.ftLastWriteTime;

    ULARGE_INTEGER vertexLarge;
    vertexLarge.LowPart = vertexTime.dwLowDateTime;
    vertexLarge.HighPart = vertexTime.dwHighDateTime;
    ULONGLONG currentVertexTime = vertexLarge.QuadPart;

    ULARGE_INTEGER fragmentLarge;
    fragmentLarge.LowPart = fragmentTime.dwLowDateTime;
    fragmentLarge.HighPart = fragmentTime.dwHighDateTime;
    ULONGLONG currentFragmentTime = fragmentLarge.QuadPart;

    bool hasChanged = currentVertexTime != lastVertexTime || currentFragmentTime != lastFragmentTime;
    if(hasChanged)
    {
        LoadShader((char*)vertexPath, (char*)fragmentPath);
        lastVertexTime = (long)currentVertexTime;
        lastFragmentTime = (long)currentFragmentTime;
    }

    return hasChanged;
}

// NOTE: This function does not work correctly on non-Linux operating systems
/* #include <sys/stat.h>   // Only on POSIX/Linux
bool Shader::CheckChanged(const char* vertexPath, const char* fragmentPath)
{
    struct stat sb;

    lstat(vertexPath, &sb);
    long vertexTime = sb.st_mtime;

    lstat(fragmentPath, &sb);
    long fragmentTime = sb.st_mtime;

    bool hasChanged = lastVertexTime != vertexTime || lastFragmentTime != fragmentTime;
    if(hasChanged)
    {
        LoadShader(vertexPath, fragmentPath);
        lastVertexTime = vertexTime;
        lastFragmentTime = fragmentTime;
    }

    return hasChanged;
}
*/