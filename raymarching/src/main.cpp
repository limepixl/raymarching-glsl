#include <glad/glad.h>
#include "Shader/shader.hpp"
#include "Display/display.hpp"
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cmath>

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

    int loc1 = glGetUniformLocation(basicShader.ID, "windowSize");
    int loc2 = glGetUniformLocation(basicShader.ID, "time");
    int loc3 = glGetUniformLocation(basicShader.ID, "cameraPosition");
    int loc4 = glGetUniformLocation(basicShader.ID, "forward");
    int loc5 = glGetUniformLocation(basicShader.ID, "right");
    int loc6 = glGetUniformLocation(basicShader.ID, "up");

    double lastX, lastY;
    double xpos = 0.0, ypos = 0.0;  // Used for storing mouse position
    glm::vec3 camPosition(0.0, 1.0, 5.0);

    float sensitivity = 0.2f;
    float yaw = 0.0;
    float pitch = 0.0;

    // Render loop
    while(display.IsOpen())
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        lastX = xpos; lastY = ypos;
        glfwGetCursorPos(display.window, &xpos, &ypos);
        
        float xoffset = float(xpos - lastX);
        float yoffset = float(lastY - ypos);
        yaw += xoffset * sensitivity; 
        pitch += yoffset * sensitivity; 

        if(pitch > 89.0f)
            pitch = 89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        glm::vec3 forward = glm::normalize(front);
        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0, 1.0, 0.0)));
        glm::vec3 up = glm::normalize(glm::cross(right, forward));

        // Moving around
        float speed = 5.0f * (float)display.deltaTime;
        if(glfwGetKey(display.window, GLFW_KEY_W) == GLFW_PRESS)
            camPosition += forward * speed;
        if(glfwGetKey(display.window, GLFW_KEY_S) == GLFW_PRESS)
            camPosition -= forward * speed;
        if(glfwGetKey(display.window, GLFW_KEY_A) == GLFW_PRESS)
            camPosition -= right * speed;
        if(glfwGetKey(display.window, GLFW_KEY_D) == GLFW_PRESS)
            camPosition += right * speed;
        if(glfwGetKey(display.window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camPosition += glm::vec3(0.0, 1.0, 0.0) * speed;
        if(glfwGetKey(display.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            camPosition -= glm::vec3(0.0, 1.0, 0.0) * speed;

        glUniform3f(loc3, camPosition.x, camPosition.y, camPosition.z); // Camera position
        glUniform3f(loc4, forward.x, forward.y, forward.z);             // Camera forward
        glUniform3f(loc5, right.x, right.y, right.z);                   // Camera right
        glUniform3f(loc6, up.x, up.y, up.z);                            // Camera up
        glUniform1f(loc2, (float)glfwGetTime());                        // Time
        glUniform2f(loc1, (float)WIDTH, (float)HEIGHT);                 // Screen size

        display.Update();
    }

    return 0;
}