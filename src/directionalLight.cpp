#include "directionalLight.h"

DirectionalLight::DirectionalLight() : Light() // before it goes into the function code, it will call Light() where we set some parameters
{
    direction = glm::vec3(0.0f, -1.0f, 0.0f); 
    lightProj = glm::ortho(-5.0f, 5.0f,-5.0f, 5.0f, 0.1f, 100.0f);  // creates a camera with no perspective
    // args: range left, range right, range bootom, range top, near plane, far plane
}

DirectionalLight::DirectionalLight( GLuint shadowWidth, GLuint shadowHeight,
                                    GLfloat red, GLfloat green, GLfloat blue,
                                    GLfloat aIntensity,  GLfloat dIntensity, 
                                    GLfloat xDir, GLfloat yDir, GLfloat zDir) : Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, dIntensity)
{
    direction = glm::normalize(glm::vec3(xDir, yDir, zDir)); 
    lightProj = glm::ortho(-20.0f, 20.0f,-20.0f, 20.0f, 0.1f, 100.0f);
}

void DirectionalLight::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation,  GLfloat diffuseIntensityLocation, GLfloat directionLocation){

    glUniform3f(ambientColorLocation, color.x, color.y, color.z); // in the shader we will have a vec3 and it will be bound to ambientColorLocation
    glUniform1f(ambientIntensityLocation, ambientIntensity); 

    glUniform3f(directionLocation, direction.x, direction.y, direction.z); 
    glUniform1f(diffuseIntensityLocation, diffuseIntensity); 
}

glm::mat4 DirectionalLight::CalculateLightTransform(){

    return lightProj * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));  
    // the model is on the shader so we only do projection and view here
    // to get the view we use glm::lookAt and we take the opposite of the direction, for the fragment being lit the lihgh is coming from the opposite
    // of the direction => We are here faking a position 
    // and we want to look at the origin: glm::vec3(0.0f, 0.0f, 0.0f) SO we are going from the oposite of the direction to the origin which is simply the direction. 
    // we know that the directional light is the direction that the origin is being shown at from so we know that the origin will be in the direction that light is facing
    // the last arg is "up". 

}

DirectionalLight::~DirectionalLight(){}