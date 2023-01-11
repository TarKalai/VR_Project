#pragma once

#include <glad/glad.h>
#include "stb_image.h"


class Texture{

public: 
    const char* name;
    Texture(); 
    Texture(const char* fileLoc, const char* _name); 

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