#pragma once
#include <stdio.h>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

struct Resolution {
    int width;
    int height;
};

struct Point {
    int x;
    int y;
};

class Display{
public: 
    bool cursor_disabled;

    Display() {cursor_disabled = false;}
    Display(bool cursor); 

    int Initialise(); 

    GLfloat getBufferWidth(){return bufferWidth;}
    GLfloat getBufferHeight(){return bufferHeight;}

    
    void static framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void static scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    bool getShouldClose(){return glfwWindowShouldClose(mainWindow); }
    void swapBuffers(){glfwSwapBuffers(mainWindow);}

    
    GLFWwindow* getWindow(){return mainWindow;}
    bool getCursorDisabled(){return cursor_disabled;}

    Point getCenter();
    ~Display(); 


private: 
    GLFWwindow *mainWindow; 
    GLint width, height; 
    GLint bufferWidth, bufferHeight; 
};