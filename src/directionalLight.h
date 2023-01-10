#pragma once

#include "light.h"
#include "constant.h"
#include "utils.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>


class DirectionalLight:
    public Light
{   
public : 
    DirectionalLight(); 

    DirectionalLight(GLfloat shadowWidth, GLfloat shadowHeight, 
                    GLfloat red, GLfloat green, GLfloat blue,
                    GLfloat aIntensity , GLfloat dIntensity,
                    GLfloat xDir, GLfloat yDir, GLfloat zDir); 

    void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation);

    glm::mat4 CalculateLightTransform(); 

    ~DirectionalLight(); 

private:
    float deltaSpeed = 0;
    glm::vec3 direction; 

    
};