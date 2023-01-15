#pragma once


#include "light.h"
#include "utils.h"


class PointLight :

    public Light
{
public: 
    PointLight(); 

    PointLight(GLfloat red, GLfloat green, GLfloat blue, 
        GLfloat aIntensity, GLfloat dIntensity,
        GLfloat xPos, GLfloat yPos, GLfloat zPos, 
        GLfloat con, GLfloat lin, GLfloat exp); 
    
    void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, 
                GLuint diffuseIntensityLocation, GLuint positionLocation,
                GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation); 

    void setPosition(glm::vec3 Position){position=Position;}


    ~PointLight(); 

protected: 
    glm::vec3 position; // where the light is coming from

    GLfloat constant, linear, exponent; //ax² + bx + c; cosntant = c, linear = b, exponent =a; 
};