#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    const int WIDTH = 800;
    const int HEIGHT = 600;
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GLSL viewer", nullptr, nullptr);
    if(window == nullptr)
    {
        printf("Failed to create GLFW window!\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD!\n");
        glfwTerminate();
        return -1;
    }
    glViewport(0, 0, WIDTH, HEIGHT);
    glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // render

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}