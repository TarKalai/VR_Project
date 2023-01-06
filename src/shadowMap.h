#pragma once

#include <stdio.h>

#include <glad/glad.h>


class ShadowMap{

public: 

    ShadowMap(); 

    virtual bool Init(GLuint width, GLuint height); // initialiser : we are setting up the texture

    virtual void Write(); // bind the buffer object so that it is ready to write to the shadow map. 
    
    virtual void Read(GLenum textureUnit); // we are going to have more than one texture, so we precise which texture unit we are going to use

    GLuint GetShadowWidth(){return shadowWidth;}

    GLuint GetShadowHeight(){return shadowHeight;}

    ~ShadowMap(); 

private: 

    GLuint FBO, shadowMap;  // Id of texture
    GLuint shadowWidth, shadowHeight; // need this when we change the size of the veiewport, when we do the shadow render (first render from the point of view of the camera) we want to make sure that the viewport is the same dimensions as the shadow map texture

};