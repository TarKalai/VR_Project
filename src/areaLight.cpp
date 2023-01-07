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
        glm::vec3 Rotation, bool TwoSided, std::vector<glm::vec3> VertexPosition) : Light(red, green, blue, aIntensity, dIntensity)
{
    position = Position; 
    constant = con; 
    linear = lin; 
    exponent = exp; 
    rotation = Rotation;
    twoSided = TwoSided;
    vertexPosition = VertexPosition;
}

void AreaLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, 
                GLuint diffuseIntensityLocation, GLuint positionLocation,
                GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation)
{
    glUniform3f(ambientColorLocation, color.x, color.y, color.z); // in the shader we will have a vec3 and it will be bound to ambientColorLocation
    glUniform1f(ambientIntensityLocation, ambientIntensity); 
    glUniform1f(diffuseIntensityLocation, diffuseIntensity); 

    glUniform3f(positionLocation, position.x, position.y, position.z); 
    glUniform1f(constantLocation, constant); 
    glUniform1f(linearLocation, linear); 
    glUniform1f(exponentLocation, exponent); 

}

std::vector<glm::vec3> AreaLight::getVertexPosition(){
    return vertexPosition;
}

glm::vec3 AreaLight::getPosition(){
    return position;
}

glm::vec3 AreaLight::getRotation(){
    return rotation;
}

AreaLight::~AreaLight(){}