#pragma once
#include <iostream>
#include <stdio.h>
// #include "camera.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "physics.h"

class Camera; 

class Process{
public: 
    Process(); 
    void initMousePosition(GLFWwindow* window, Camera &camera, bool cursor_disabled);
    void processInput(GLFWwindow* window, Camera &camera, PhysicalWorld &world, Shader &shader); 
    void HandleMouse(GLFWwindow* window, Camera &camera); 
    void processMouseScroll(float yoffset, Camera &camera); 
    void SaveCursorPath(GLFWwindow* window, Camera &camera, PhysicalWorld &world, Shader &shader);
    void PutDominoes(GLFWwindow* window, Camera &camera, PhysicalWorld &world, Shader &shader);
    ~Process(); 
private: 
    bool fullscreen = false;
    bool resizescreen = false;
    int pressed = 0;
    bool shoot = false;
    std::vector<glm::vec3> cursor_path;
}; 