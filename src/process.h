#pragma once

#include <iostream>
#include <stdio.h>
#include "string.h"

#include <unistd.h> // for sleep (debug)

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtx/string_cast.hpp"

#include "shader.h"
#include "physics.h"
#include "camera.h"
#include "display.h"
#include "constant.h"
#include "utils.h"
#include "texture.h"
#include "material.h"

class Camera; 

class Process{
public: 
    Display* display;
    GLFWwindow* window; 
    Camera* camera;
    PhysicalWorld* world;
    Shader* shader;
    Shader* shadow;
    Shader* omniShadow;
    Shader* bumpmap;
    Shader* parallax;
    
    int enterPressed = 0;
    float sliderSpeedAnimation = 1.;
    float scaleDomino = 1.;
    Object* selectedDomino;
    glm::vec3 colorDomino = glm::vec3(1.);
    Texture* textureDomino = Textures::White();
    Material* materialDomino = Materials::Shiny();
    glm::vec3 dimDomino = glm::vec3(dominoDim::width, dominoDim::height, dominoDim::thick);
    int geometryDomino = geometry::domino;

    Process(Display* displayArg, Camera* cameraArg, PhysicalWorld* worldArg, Shader* shaderArg, Shader* shadowArg, Shader* omniShadowShader,  Shader* bumpMapShader, Shader* paralaxMapShader);
    void initMousePosition();
    void processInput(); 
    void HandleMouse(); 
    void DeleteDominos();
    void ModifyDomino();
    void PutDominos();
	void AnimationSpeed();
    ~Process(); 
private: 
    bool modifyPressed = false;
    bool menuPressed = false;
    double oldCursorX = 0, oldCursorY = 0;
    int oldWindowX = 0, oldWindowY = 0;
    int screenSize = 5;
    bool fullscreen = false;
    bool resizescreen = false;
    bool firstDomino = true;
    glm::vec3 lastDomino;
    bool shoot = false;
    bool decreaseResolution = false;
    bool increaseResolution = false;
    bool scaleIncrease = false;
    bool scaleDecrease = false;
    std::vector<glm::vec3> cursor_path;

    void HandleMenuMode();
	void HandleWindow();
	void QuitScreen();
	void DecreaseScreen();
	void IncreaseScreen();
	void FullScreen();
	void PlacingParameter();
	void PlacingDomino();
	void Pushing();
	void Deplacement();
    void FlashLight();
}; 