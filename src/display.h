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

    ~Display(); 



private: 

    GLFWwindow *mainWindow; 
    GLint width, height; 
    GLint bufferWidth, bufferHeight; 


};