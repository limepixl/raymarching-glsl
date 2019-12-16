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
    const char* vertexPath = "res/shaders/Mandelbrot/vertex.glsl";
    const char* fragmentPath = "res/shaders/Mandelbrot/fragment.glsl";

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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    int loc1 = glGetUniformLocation(basicShader.ID, "windowSize");   // Window size uniform
    int loc2 = glGetUniformLocation(basicShader.ID, "mousePos");
    int loc3 = glGetUniformLocation(basicShader.ID, "scale");

    float xOffset = 0.0f;
    float yOffset = 0.0f;
    float lastX = WIDTH / 2.0f;
    float lastY = HEIGHT / 2.0f;

    float scale = 1.0f;

    // Render loop
    while(display.IsOpen())
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

        if(basicShader.CheckChanged(vertexPath, fragmentPath))
            basicShader.UseShader();
        
        double xpos, ypos;
        glfwGetCursorPos(display.window, &xpos, &ypos);

        if(glfwGetMouseButton(display.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            xOffset += xpos - lastX;
            yOffset += lastY - ypos;
        }

        lastX = xpos;
        lastY = ypos;

        if(glfwGetKey(display.window, GLFW_KEY_KP_ADD))
            scale++;
        else if(glfwGetKey(display.window, GLFW_KEY_KP_SUBTRACT))
            scale--;
        
        glUniform1f(loc3, sqrt(scale) / 10.0f);

        // Pass screen coordinates to shader
        glUniform2f(loc1, (float) WIDTH, (float) HEIGHT);

        // Pass mouse offset to shader
        glUniform2f(loc2, xOffset, yOffset);

        display.Update();
    }

    return 0;
}