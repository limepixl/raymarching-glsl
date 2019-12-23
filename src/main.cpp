#include <glad/glad.h>
#include "Shader/shader.hpp"
#include "Display/display.hpp"
#include "Math/math.hpp"
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cmath>

int main()
{
    const int WIDTH = 1000;
    const int HEIGHT = 1000;
    Display display(WIDTH, HEIGHT);

    // Shader paths
    const char* vertexPath = "../../../res/shaders/vertex.glsl";
    const char* fragmentPath = "../../../res/shaders/fragment.glsl";

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
    int loc3 = glGetUniformLocation(basicShader.ID, "cameraPosition");     // Mouse position

    double xpos = 0.0, ypos = 0.0;  // Used for storing mouse position
    Vec3 camPosition(0.0, 1.0, 0.0);
    Vec3 camDirection(0.0, 0.0, -1.0);
    Vec3 camRight(1.0, 0.0, 0.0);
    Vec3 camUp(0.0, 1.0, 0.0);

    // Render loop
    while(display.IsOpen())
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        if(basicShader.CheckChanged(vertexPath, fragmentPath))
            basicShader.UseShader();

        // Moving around
        float speed = 5.0 * display.deltaTime;
        if(glfwGetKey(display.window, GLFW_KEY_W) == GLFW_PRESS)
            camPosition += camDirection * speed;
        if(glfwGetKey(display.window, GLFW_KEY_S) == GLFW_PRESS)
            camPosition -= camDirection * speed;
        if(glfwGetKey(display.window, GLFW_KEY_A) == GLFW_PRESS)
            camPosition -= camRight * speed;
        if(glfwGetKey(display.window, GLFW_KEY_D) == GLFW_PRESS)
            camPosition += camRight * speed;
        if(glfwGetKey(display.window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camPosition += camUp * speed;
        if(glfwGetKey(display.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            camPosition -= camUp * speed;

        // Pass camera position to shader
        glUniform3f(loc3, camPosition.x, camPosition.y, camPosition.z);
        
        // Pass screen coordinates to shader
        glUniform2f(loc1, (float)WIDTH, (float)HEIGHT);

        // Pass glfw time to shader
        glUniform1f(loc2, (float)glfwGetTime());

        display.Update();
    }

    return 0;
}