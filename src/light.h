
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class Light{

public: 
    Light(); 

    Light(GLfloat red, GLfloat green, GLfloat blue, 
        GLfloat aIntensity, GLfloat dIntensity); 
    glm::vec3 getColor(){return color;}
    GLfloat getAmbientIntensity(){return ambientIntensity;}
    GLfloat getDiffuseIntensity(){return diffuseIntensity;}
    ~Light(); 


protected: 
    glm::vec3 color; 
    GLfloat ambientIntensity; 

    glm::vec3 direction; //direction of the diffuse light
    GLfloat diffuseIntensity; // how much diffuse lighting we are going to see. 


}; 