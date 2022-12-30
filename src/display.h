#pragma once
#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>



class Display{
public: 

    Display();
    Display(bool cursor); 
    Display(GLint windowWidth, GLint windowHeight, bool cursor); 

    int Initialise(); 

    
    void static framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void static scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    GLfloat getBufferWidth(){return bufferWidth;}
    GLfloat getBufferHeight(){return bufferHeight;}

    bool getShouldClose(){return glfwWindowShouldClose(mainWindow); }
    void swapBuffers(){glfwSwapBuffers(mainWindow);}

    GLFWwindow* getWindow(){return mainWindow;}
    bool getCursorDisabled(){return cursor_disabled;}
    
    ~Display(); 


private: 

    GLFWwindow *mainWindow; 
    GLint width, height; 
    GLint bufferWidth, bufferHeight; 
    bool cursor_disabled;
    


};