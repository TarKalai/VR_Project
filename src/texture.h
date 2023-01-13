#pragma once

#include <glad/glad.h>
#include "stb_image.h"
#include <GLFW/glfw3.h>

class Texture{

public: 
    const char* name;
    Texture(); 
    Texture(const char* FileLocation, const char* _name, const char * NormalLocation=nullptr);
    GLuint LoadTexture(const char* FileLoc, bool flip=true);
    void UseTexture(); 
    void ClearTexture(); 
    int getWidth(){return width;}

    ~Texture(); 

private: 

    GLuint textureID, normalID; 
    int width, height, nrComponents; 
};