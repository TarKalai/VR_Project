#pragma once
#include <iostream>
#include <stdio.h>
// #include "camera.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "physics.h"
#include "camera.h"
#include "display.h"
class Camera; 

class Process{
public: 
    Display* display;
    GLFWwindow* window; 
    Camera* camera;
    PhysicalWorld* world;
    Shader* shader;
    float oldSpeedAnimation;

    Process(Display* displayArg, Camera* cameraArg, PhysicalWorld* worldArg, Shader* shaderArg); 
    void initMousePosition();
    void processInput(); 
    void HandleMouse(); 
    void processMouseScroll(float yoffset);
    void PutDominoes();
    ~Process(); 
private: 
    bool menuPressed = false;
    double oldCursorX = 0, oldCursorY = 0;
    int oldWindowX = 0, oldWindowY = 0;
    int screenSize = 5;
    bool fullscreen = false;
    bool resizescreen = false;
    bool firstDomino = true;
    glm::vec3 lastDomino;
    bool shoot = false;
    int pressed = 0;
    bool decreaseResolution = false;
    bool increaseResolution = false;
    std::vector<glm::vec3> cursor_path;

    void HandleMenuMode();
	void HandleWindow();
	void QuitScreen();
	void DecreaseScreen();
	void IncreaseScreen();
	void FullScreen();
	void AnimationSpeed();
	void PlacingDomino();
	void Pushing();
	void Deplacement();
}; 