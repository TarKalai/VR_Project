#pragma once

#include <glad/glad.h>
#include "stb_image.h"


class Texture{

public: 
    Texture(); 
    Texture(const char* fileLoc); 

    bool LoadTexture();
    void UseTexture(); 
    void ClearTexture(); 

    ~Texture(); 

private: 

    GLuint textureID; 
    int width, height, bitDepth; 

    const char* fileLocation;  

};