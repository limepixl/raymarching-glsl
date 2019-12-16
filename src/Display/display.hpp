#pragma once

struct Display
{
    int width;
    int height;

    struct GLFWwindow* window;

    Display(int w, int h);
    ~Display();

    bool IsOpen();
    void Update();
};