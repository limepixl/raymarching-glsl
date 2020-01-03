#pragma once

struct Shader
{
    unsigned int ID;

    void LoadShader(const char* vertexPath, const char* fragmentPath);
    void UseShader();
};