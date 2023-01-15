#include "pointLight.h"

PointLight::PointLight() : Light()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f); 
    constant = 1.0f; 
    linear = 0.0f; 
    exponent = 0.0f; 
}

PointLight::PointLight(GLuint shadowWidth, GLuint shadowHeight,
        GLfloat near, GLfloat far,
        GLfloat red, GLfloat green, GLfloat blue, 
        GLfloat aIntensity, GLfloat dIntensity,
        GLfloat xPos, GLfloat yPos, GLfloat zPos, 
        GLfloat con, GLfloat lin, GLfloat exp) : Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, dIntensity)
{
    position = glm::vec3(xPos, yPos, zPos); 
    constant = con; 
    linear = lin; 
    exponent = exp; 

    farPlane = far; 

    float aspect = (float)shadowWidth/(float)shadowHeight; 
    // it should be 1, we want the shadowWidth = shadowHeight otherwise the shadows may look strange

    lightProj = glm::perspective(glm::radians(90.0f), aspect, near, (GLfloat)far); // we will have a bunch of 90° views combined => cube 

    shadowMap = new OmniShadowMap();    
    shadowMap->Init(shadowWidth, shadowHeight); 

}

void PointLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, 
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

std::vector<glm::mat4> PointLight::CalculateLightTransform()
{
    std::vector<glm::mat4> lightMatrices; 
    // need to make sure it is the correct order : pos_x, neg_x, pos_y, neg_y, pos_z, neg_z
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0))); // pos_X CHECK IF IT IS -1 or 1 !
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0))); // neg X

    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0))); // pos Y
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0))); // neg Y

    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0))); // pos Z
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0))); // neg Z

    // remark last arg : for x and y the light come from the up position (so -& for y) but for y it will depend where we look.

    return lightMatrices; 

}

GLfloat PointLight::GetFarPlane()
{
    return farPlane; 
}


glm::vec3 PointLight::GetPosition(){
    return position; 
}

PointLight::~PointLight(){}