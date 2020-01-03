#include "display.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>

Display::Display(int width, int height) : width(width), height(height)
{
    // GLFW init
    glfwInit();
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

    window = glfwCreateWindow(width, height, "GLSL viewer", nullptr, nullptr);
    if(window == nullptr)
    {
        printf("Failed to create GLFW window!\n");
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Bind cursor within screen 

    // GLAD init and viewport config
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD!\n");
        glfwTerminate();
    }
    glViewport(0, 0, width, height);
    glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

    // Used for delta time calculations
    deltaTime = 0.0f;
    lastTime = 0.0f;
}

Display::~Display()
{
    glfwTerminate();
}

bool Display::IsOpen()
{
    return !glfwWindowShouldClose(window);
}

void Display::Update()
{
    // Calculate deltaTime
    float currentTime = (float)glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    glfwSwapBuffers(window);
    glfwPollEvents();
}