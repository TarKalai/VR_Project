
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shadowMap.h"

class Light{

public: 
    Light(); 

    Light(GLuint shadowWidth, GLuint shadowHeight, 
        GLfloat red, GLfloat green, GLfloat blue, 
        GLfloat aIntensity, GLfloat dIntensity); 
    
    ShadowMap* GetShadowMap(){return shadowMap; }


    ~Light(); 


protected: 
    glm::vec3 color; 
    GLfloat ambientIntensity; 

    glm::vec3 direction; //direction of the diffuse light
    GLfloat diffuseIntensity; // how much diffuse lighting we are going to see. 

    
    glm::mat4 lightProj; // how the light see

    ShadowMap* shadowMap; 


}; 