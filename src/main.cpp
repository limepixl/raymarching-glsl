#include <glad/glad.h>
#include "Shader/shader.hpp"
#include "Display/display.hpp"
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cmath>

int main()
{
    const int WIDTH = 1000;
    const int HEIGHT = 1000;
    Display display(WIDTH, HEIGHT);

    // Shader paths
    const char* vertexPath = "res/shaders/vertex.glsl";
    const char* fragmentPath = "res/shaders/fragment.glsl";

    Shader basicShader;
    basicShader.LoadShader(vertexPath, fragmentPath);
    basicShader.UseShader();

    float positions[]
    {
        -1.0f, -1.0f,
         1.0f, -1.0f,
         1.0f,  1.0f,
         1.0f,  1.0f,
        -1.0f,  1.0f,
        -1.0f, -1.0f
    };

    // Passing vertex positions to shaders
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    int loc1 = glGetUniformLocation(basicShader.ID, "windowSize");   // Window size uniform
    int loc2 = glGetUniformLocation(basicShader.ID, "time");         // Time

    // Render loop
    while(display.IsOpen())
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        if(basicShader.CheckChanged(vertexPath, fragmentPath))
            basicShader.UseShader();

        // Pass screen coordinates to shader
        glUniform2f(loc1, (float) WIDTH, (float) HEIGHT);

        // Pass glfw time to shader
        glUniform1f(loc2, (float) glfwGetTime());

        display.Update();
    }

    return 0;
}