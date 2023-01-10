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
    int getWidth(){return width;}

    ~Texture(); 

private: 

    GLuint textureID; 
    int width, height, nrComponents; 

    const char* fileLocation;  

};