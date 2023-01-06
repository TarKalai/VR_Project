#pragma once
#include "process.h"
#include "display.h"

class GUI {
public:
    GUI(Display* displayArg, PhysicalWorld* worldArg);
    void update();
    ~GUI(); 

private:
    Display* display;
    PhysicalWorld* world;
    double prev = 0;
	int deltaFrame = 0;
    float fps = 0;

    float* getFPS();
    void displayFPS();
    void displaySaveLoad();
    void displaySpeedAnimation();
};