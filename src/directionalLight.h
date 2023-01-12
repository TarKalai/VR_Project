#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "light.h"
#include "constant.h"
#include "utils.h"


class DirectionalLight:
    public Light
{   
public : 
    DirectionalLight(); 

    DirectionalLight(GLfloat shadowWidth, GLfloat shadowHeight, 
                    GLfloat red, GLfloat green, GLfloat blue,
                    GLfloat aIntensity , GLfloat dIntensity,
                    GLfloat xDir, GLfloat yDir, GLfloat zDir); 

    void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseIntensityLocation, GLuint directionLocation);

    glm::mat4 CalculateLightTransform(); 

    ~DirectionalLight(); 

private:
    float x,y,z;
    float deltaSpeed = 0;
};