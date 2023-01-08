#include "areaLight.h"

AreaLight::AreaLight() : Light()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f); 
    constant = 1.0f; 
    linear = 0.0f; 
    exponent = 0.0f; 
    rotation = glm::vec3(0.0);
    twoSided = true;
    
}

AreaLight::AreaLight(GLfloat red, GLfloat green, GLfloat blue, 
        GLfloat aIntensity, GLfloat dIntensity,
        glm::vec3 Position, 
        GLfloat con, GLfloat lin, GLfloat exp,
        glm::vec3 Rotation, bool TwoSided, std::vector<glm::vec3> VertexPosition, glm::vec3 Scale) : Light(red, green, blue, aIntensity, dIntensity)
{
    position = Position; 
    constant = con; 
    linear = lin; 
    exponent = exp; 
    rotation = Rotation;
    twoSided = TwoSided;
    vertexPosition = VertexPosition;
    scale = Scale;
}



AreaLight::~AreaLight(){}