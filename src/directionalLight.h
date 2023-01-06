#pragma once

#include "light.h"
class DirectionalLight:
    public Light
{   
public : 
    DirectionalLight(); 

    DirectionalLight(GLuint shadowWidth, GLuint shadowHeight, 
                    GLfloat red, GLfloat green, GLfloat blue,
                    GLfloat aIntensity , GLfloat dIntensity,
                    GLfloat xDir, GLfloat yDir, GLfloat zDir); 

    void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation);

    glm::mat4 CalculateLightTransform(); 


    ~DirectionalLight(); 

private:

    glm::vec3 direction; 

    
};