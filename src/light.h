
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include "shadowMap.h"

class Light{

public: 
    Light(); 

    Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity);

    Light(GLfloat shadowWidth, GLfloat shadowHeight,
        GLfloat red, GLfloat green, GLfloat blue, 
        GLfloat aIntensity, GLfloat dIntensity); 

    ShadowMap* GetShadowMap(){return shadowMap;}
    GLfloat    getAmbientIntensity(){return ambientIntensity;}
    GLfloat    getDiffuseIntensity(){return diffuseIntensity;}
    glm::vec3  getColor(){return color;}
    glm::vec3  getDirection(){return direction;}


    ~Light(); 


protected: 
    glm::vec3 color; 
    GLfloat ambientIntensity; 

    glm::vec3 direction; //direction of the diffuse light
    GLfloat diffuseIntensity; // how much diffuse lighting we are going to see. 

    glm::mat4 lightProj; // how the light see

    ShadowMap* shadowMap; 


}; 