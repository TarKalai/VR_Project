#pragma once


#include "light.h"


class AreaLight :

    public Light
{
public: 
    AreaLight(); 

    AreaLight(GLfloat red, GLfloat green, GLfloat blue, 
        GLfloat aIntensity, GLfloat dIntensity,
        GLfloat xPos, GLfloat yPos, GLfloat zPos, 
        GLfloat con, GLfloat lin, GLfloat exp); 
    
    void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, 
                GLuint diffuseIntensityLocation, GLuint positionLocation,
                GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation); 


    ~AreaLight(); 

protected: 
    glm::vec3 position; // where the light is coming from

    GLfloat constant, linear, exponent; //ax² + bx + c; cosntant = c, linear = b, exponent =a; 
};