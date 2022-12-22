

#include <stdio.h>
// #include "camera.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera; 

class Process{
public: 
    Process(); 
    void processInput(GLFWwindow* window, Camera &camera); 
    void HandleMouse(GLFWwindow* window, Camera &camera); 
    void processMouseScroll(float yoffset, Camera &camera); 
    ~Process(); 
private: 
}; 