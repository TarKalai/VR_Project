#pragma once


#include "light.h"
#include <vector>

class AreaLight :

    public Light
{
public: 
    AreaLight(); 

    AreaLight(GLfloat red, GLfloat green, GLfloat blue, 
        GLfloat aIntensity, GLfloat dIntensity,
        glm::vec3 Position, 
        GLfloat con, GLfloat lin, GLfloat exp, 
        glm::vec3 Rotation, bool TwoSided, std::vector<glm::vec3> VertexPosition); 
    
    void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, 
                GLuint diffuseIntensityLocation, GLuint positionLocation,
                GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation); 

    std::vector<glm::vec3> getVertexPosition();
    glm::vec3 getPosition();
    glm::vec3 getRotation();
 
    ~AreaLight(); 

protected: 
    glm::vec3 position; // where the light is coming from
    glm::vec3 rotation;
    GLfloat constant, linear, exponent; //ax² + bx + c; cosntant = c, linear = b, exponent =a; 
    bool twoSided;
    std::vector<glm::vec3> vertexPosition;
};