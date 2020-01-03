#pragma once

struct Shader
{
    long lastVertexTime = 0;
    long lastFragmentTime = 0;
    unsigned int ID;

    void LoadShader(const char* vertexPath, const char* fragmentPath);
    void UseShader();
    bool CheckChanged(const wchar_t* vertexPath, const wchar_t* fragmentPath);
};