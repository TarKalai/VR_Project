#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>



class Display{
public: 
    Display(); 
    Display(GLint windowWidth, GLint windowHeight); 

    int Initialise(); 

    GLfloat getBufferWidth(){return bufferWidth;}
    GLfloat getBufferHeight(){return bufferHeight;}

    bool getShouldClose(){return glfwWindowShouldClose(mainWindow); }
    void swapBuffers(){glfwSwapBuffers(mainWindow);}

    GLFWwindow* getWindow(){return mainWindow;}
    void static framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void static scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    ~Display(); 
    bool firstmouse; 




private: 

    GLFWwindow *mainWindow; 
    GLint width, height; 
    GLint bufferWidth, bufferHeight; 
    


};