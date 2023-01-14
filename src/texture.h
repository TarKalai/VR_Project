#pragma once

#include <glad/glad.h>
#include "stb_image.h"
#include <GLFW/glfw3.h>

class Texture{

public: 
    const char* name;
    Texture(); 
    Texture(const char* FileLocation, const char* _name, bool mirrored_x, bool mirrored_y, const char * NormalLocation=nullptr, const char * DispLocation=nullptr);
    GLuint LoadTexture(const char* FileLoc, bool flip, bool mirrored_x, bool mirrored_y);
    void UseTexture(); 
    void ClearTexture(); 
    int getWidth(){return width;}

    ~Texture(); 

private: 

    GLuint textureID, normalID, dispID; 
    int width, height, nrComponents; 
};