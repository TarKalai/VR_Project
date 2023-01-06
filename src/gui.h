#pragma once
#include "process.h"
#include "display.h"

#include <dirent.h> // Linux
#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>

class GUI {
public:
    GUI(Process* processArg, Display* displayArg, PhysicalWorld* worldArg);
    void update();
    void clear();

private:
    Process* process;
    Display* display;
    PhysicalWorld* world;
    double prev = 0;
	int deltaFrame = 0;
    float fps = 0;

    void menuTitle();
    void shortcutList();
    float* getFPS();
    void displayFPS();
    void displaySaveLoad();
    void displaySpeedAnimation();
};