#pragma once

#include <glad/glad.h>
#include "stb_image.h"


class Texture{

public: 
    Texture(); 
    Texture(const char* fileLoc); 

    bool LoadTexture(); // use a boolean to know if the texture has well been loaded. If the texture load failed then we will attach a default texture 
    bool LoadTextureA(); // for non-alpha channel texture  
    void UseTexture(); 
    void ClearTexture(); 

    ~Texture(); 

private: 

    GLuint textureID; 
    int width, height, bitDepth; 

    const char* fileLocation;  

};