#pragma once
#include <vector>


#include "light.h"
#include "omniShadowMap.h"

class PointLight :

    public Light
{
public: 
    PointLight(); 

    PointLight(GLuint shadowWidth, GLuint shadowHeight,
        GLfloat near, GLfloat far, 
        GLfloat red, GLfloat green, GLfloat blue, 
        GLfloat aIntensity, GLfloat dIntensity,
        GLfloat xPos, GLfloat yPos, GLfloat zPos, 
        GLfloat con, GLfloat lin, GLfloat exp); 
    
    void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, 
                GLuint diffuseIntensityLocation, GLuint positionLocation,
                GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation); 
    
    std::vector<glm::mat4> CalculateLightTransform(); 

    GLfloat GetFarPlane(); 

    glm::vec3 GetPosition(); 



    void setPosition(glm::vec3 Position){position=Position;}


    ~PointLight(); 

protected: 
    glm::vec3 position; // where the light is coming from

    GLfloat constant, linear, exponent; //ax² + bx + c; cosntant = c, linear = b, exponent =a; 

    GLfloat farPlane; 
};