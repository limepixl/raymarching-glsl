#include <glad/glad.h>
#include "Shader/shader.hpp"
#include "Display/display.hpp"

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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    int loc = glGetUniformLocation(basicShader.ID, "windowSize");   // Window size uniform
    int loc2 = glGetUniformLocation(basicShader.ID, "borders");     // Mandelbrot borders uniform

    float x0 = -2.0f;
    float x1 = 0.5f;
    float y0 = -1.25f;
    float y1 = 1.25f;
    float diff = 0.001f;

    // Render loop
    while(display.IsOpen())
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
        if(basicShader.CheckChanged(vertexPath, fragmentPath))
            basicShader.UseShader();

        // Pass screen coordinates to shader
        glUniform2f(loc, (float) WIDTH, (float) HEIGHT);

        // Pass the borders to the shader
        glUniform4f(loc2, x0 += diff, x1 -= diff, y0 += diff, y1 -= diff);

        display.Update();
    }

    return 0;
}