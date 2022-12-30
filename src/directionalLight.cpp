#include "directionalLight.h"

DirectionalLight::DirectionalLight() : Light() // before it goes into the function code, it will call Light() where we set some parameters
{
    direction = glm::vec3(0.0f, -1.0f, 0.0f); 
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
                                    GLfloat aIntensity,  GLfloat dIntensity, 
                                    GLfloat xDir, GLfloat yDir, GLfloat zDir) : Light(red, green, blue, aIntensity, dIntensity)
{
    direction = glm::vec3(xDir, yDir, zDir); 
}

void DirectionalLight::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation,  GLfloat diffuseIntensityLocation, GLfloat directionLocation){

    glUniform3f(ambientColorLocation, color.x, color.y, color.z); // in the shader we will have a vec3 and it will be bound to ambientColorLocation
    glUniform1f(ambientIntensityLocation, ambientIntensity); 

    glUniform3f(directionLocation, direction.x, direction.y, direction.z); 
    glUniform1f(diffuseIntensityLocation, diffuseIntensity); 
}

DirectionalLight::~DirectionalLight(){}