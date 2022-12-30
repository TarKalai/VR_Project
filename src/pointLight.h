#pragma once


#include "light.h"


class PointLight :

    public Light
{
public: 
    PointLight(); 

    PointLight(GLfloat red, GLfloat green, GLfloat blue, 
        GLfloat aIntensity, GLfloat dIntensity,
        GLfloat xPos, GLfloat yPos, GLfloat zPos, 
        GLfloat con, GLfloat lin, GLfloat exp); 
    
    void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, 
                GLfloat diffuseIntensityLocation, GLfloat positionLocation,
                GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation); 


    ~PointLight(); 

private: 
    glm::vec3 position; // where the light is coming from

    GLfloat constant, linear, exponent; //ax² + bx + c; cosntant = c, linear = b, exponent =a; 
};