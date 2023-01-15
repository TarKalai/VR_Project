#pragma once

#include "shadowMap.h"

class OmniShadowMap : public ShadowMap
{
public: 
OmniShadowMap(); 

bool Init(unsigned int width, unsigned int height); // initialiser : we are setting up the texture

void Write(); // bind the buffer object so that it is ready to write to the shadow map. 
    
void Read(GLenum textureUnit); // we are going to have more than one texture, so we precise which texture unit we are going to use

~OmniShadowMap(); 

private: 

};