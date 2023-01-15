#include "omniShadowMap.h"


OmniShadowMap::OmniShadowMap() : ShadowMap(){}

bool OmniShadowMap::Init(unsigned int width, unsigned int height){

    shadowWidth = width; 
    shadowHeight = height; 

    glGenFramebuffers(1, &FBO); 

    glGenTextures(1, &shadowMap); 
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap); 
    // shadowMap is the id of what we are binding to
    // GL_TEXTURE_CUBE_MAP : we are passing 6 textures. 
    // we could pass each texture 6 times but we will use the geometry shader

    for (size_t i =0; i < 6; i ++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr); 
        // GL_TEXTURE_CUBE_MAP_POSITIVE_X : left side of the cube
    }

    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
    // With GL_LINEAR it will blend the pixels together (the other type: GL_NEAREST we will have a more pixalated look)
    // we are using GL_NEAREST for the shadow 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
    // GL_TEXTURE_MAG_FILTER: when we are closer to the image. 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //Can't have GL_REPEAT with shadows, otherwise OVERSAMPLING: the shadows will be repeated  
    // GL_CLAMP_TO_BORDER: beyond the edge of the texture there is a single color that expands on infinitely
    // if we set the border to be 1 then whatever the camera manages to see, we garantee there is going to be light beyond the border. 
    // GL_TEXTURE_WRAP_S: it defines how the texture behaves around the s axis which corresponds to the x axis, with GL_REPEAT we tell it to trepeat the texture once we go over the border. 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // The S and T axis corresponds to the u,v coordinates. (x and y)

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // R is the z axis

    glBindFramebuffer(GL_FRAMEBUFFER, FBO); 

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMap, 0); 

    glDrawBuffer(GL_NONE); 
    glReadBuffer(GL_NONE); 

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER); // get if there are errors 

    if (status != GL_FRAMEBUFFER_COMPLETE){ 

        printf("Framebuffer Error: %i\n", status); 
        return false; 

    } 

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // set to default frabe buffer the one behind the screen

    return true; 
    
}


void OmniShadowMap::Write(){
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO); // instead to draw to the default buffer, it is drawing to the one off screen
}

void OmniShadowMap::Read(GLenum textureUnit){
    glActiveTexture(textureUnit); // read from the shadowMap
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap); 

}

OmniShadowMap::~OmniShadowMap(){}