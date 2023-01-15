#pragma once
#include "process.h"
#include "display.h"
#include "constant.h"
#include "physics.h"

#include <dirent.h> // Linux
#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>
#include <sstream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "utils.h"

class GUI {
public:
    GUI(Process* processArg, Display* displayArg, PhysicalWorld* worldArg, Shader* shaderArg, Shader* shadowArg, Shader* omniShadowShader);
    void update();
    void clear();

private:
    Process* process;
    Display* display;
    PhysicalWorld* world;
    Shader* shader;
    Shader* shadow;
    Shader* omniShadow;
    double prev = 0;
	int deltaFrame = 0;
    float fps = 0;

    bool texturePicked = false;
    bool materialPicked = false;
    bool geometryPicked = false;

    void menuTitle();
    float* getFPS();
    void displayFPS();
    void displayTime();
    void displaySpeedAnimation();
    void displayDominoInfo();
    void displaySaveLoad();
    void dominoModify();
    void displayPushPower();
    void shortcutList();
};