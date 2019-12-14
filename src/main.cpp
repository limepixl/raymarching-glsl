#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include "Shader/shader.hpp"

int main()
{
    // GLFW init
    glfwInit();
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

    // Window creation
    const int WIDTH = 500;
    const int HEIGHT = 500;
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GLSL viewer", nullptr, nullptr);
    if(window == nullptr)
    {
        printf("Failed to create GLFW window!\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // GLAD init and viewport config
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD!\n");
        glfwTerminate();
        return -1;
    }
    glViewport(0, 0, WIDTH, HEIGHT);
    glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

    Shader basicShader;
    basicShader.LoadShader("res/shaders/vertex.glsl", "res/shaders/fragment.glsl");
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

    // Render loop
    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}