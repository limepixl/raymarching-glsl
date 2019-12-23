#pragma once

struct Display
{
    int width;
    int height;

    float deltaTime;
    float lastTime;

    struct GLFWwindow* window;

    Display(int w, int h);
    ~Display();

    bool IsOpen();
    void Update();
};