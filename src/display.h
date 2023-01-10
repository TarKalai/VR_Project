#pragma once

#include <stdio.h>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

struct Point {
    int x;
    int y;
};

class Display
{
public: 
    bool cursor_disabled;



    Display() {};
    Display(bool cursor); 
    // Display(GLint windowWidth, GLint windowHeight); 

    int Initialise(); 

    void static framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void static scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    GLfloat getBufferWidth(){return bufferWidth;}
    GLfloat getBufferHeight(){return bufferHeight;}

    bool getShouldClose(){return glfwWindowShouldClose(mainWindow); }
    void swapBuffers(){glfwSwapBuffers(mainWindow);}

    // bool* getsKeys(){return keys; }
    // GLfloat getXchange(); 
    // GLfloat getYchange();

    GLFWwindow* getWindow(){return mainWindow;}
    bool getCursorDisabled(){return cursor_disabled;}

    Point getCenter(); 
    int getWidth();
    int getHeight();  

    ~Display(); 

private: 
    GLFWwindow *mainWindow; 
    GLint width, height; 
    GLint bufferWidth, bufferHeight; 

    // GLfloat lastX; 
    // GLfloat lastY; 
    // GLfloat xChange; 
    // GLfloat yChange; 
    // bool mouseFirstMoved;

    // bool keys[1024]; // there are 1024 keys. 
    // //The list will consider all the keys of teh 
    // //keyboard and when one key is pressed, in the array it will be set to true, and then after pressing it will turn false. 

    // void createCallbacks(); 
    // static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode); 
    // // not using the static, make this fucntion only for one object. 
    // //But using the static will make it so we don't know on which key it will be. But we assigned a userPointer to the window so even if
    // // the function itself won't know which object called it, we will pass it the class and then we will ba able to access the keys for it
    // // key: value of the key (number between 0 and 1024)
    // // action : action performed on the key : press or release of the key. 
    // // code and mode are only there to respect the standard (otherwise it will not be recognised)

    // static void handleMouse(GLFWwindow* window, double xPos, double yPos); 


};