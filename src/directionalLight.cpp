#include "directionalLight.h"

DirectionalLight::DirectionalLight() : Light() // before it goes into the function code, it will call Light() where we set some parameters
{
    float x_size, y_size, z_size;
    x_size = 0.75*general::sceneSize.x;
    y_size = 0.75*general::sceneSize.y;
    z_size = 0.75*general::sceneSize.z;
    lightProj = glm::ortho(-x_size, x_size, -y_size, y_size, -z_size, z_size); 
}

DirectionalLight::DirectionalLight( GLfloat shadowWidth, GLfloat shadowHeight,
                                    GLfloat red, GLfloat green, GLfloat blue,
                                    GLfloat aIntensity,  GLfloat dIntensity, 
                                    GLfloat xDir, GLfloat yDir, GLfloat zDir) : Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, dIntensity)
{
    float x_size, y_size, z_size;
    x_size = 0.75*general::sceneSize.x;
    y_size = 0.75*general::sceneSize.y;
    z_size = 0.75*general::sceneSize.z;
    lightProj = glm::ortho(-x_size, x_size, -y_size, y_size, -z_size, z_size); 
}

void DirectionalLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,  GLuint diffuseIntensityLocation, GLuint directionLocation){
    // Color and intensity rendering (empirique values)
    float base = 1.5;
    glm::vec3 color = normalize(glm::vec3(base+color::OrangeRed.x*pow(x,20), base+color::OrangeRed.y*pow(x,20), base+color::OrangeRed.z*pow(x,20)));
    glUniform3f(ambientColorLocation, color.x, color.y, color.z); // in the shader we will have a vec3 and it will be bound to ambientColorLocation
    glUniform1f(ambientIntensityLocation, 0.5-y*0.25); 
    glUniform3f(directionLocation, x, y, z); 
    glUniform1f(diffuseIntensityLocation, 0.25-y*0.25); 
}


glm::mat4 DirectionalLight::CalculateLightTransform(){


    float time = Time::getTime();
    x = glm::sin(2*3.14*time/Ttime::maxTime);
    y = glm::cos(2*3.14*time/Ttime::maxTime);
    z = glm::sin(2*3.14*time/Ttime::maxTime);
    
    return lightProj * glm::lookAt(-glm::vec3(x, y, z), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));  
    // the model is on the shader so we only do projection and view here
    // to get the view we use glm::lookAt and we take the opposite of the direction, for the fragment being lit the lihgh is coming from the opposite
    // of the direction => We are here faking a position 
    // and we want to look at the origin: glm::vec3(0.0f, 0.0f, 0.0f) SO we are going from the oposite of the direction to the origin which is simply the direction. 
    // we know that the directional light is the direction that the origin is being shown at from so we know that the origin will be in the direction that light is facing
    // the last arg is "up". 


}

DirectionalLight::~DirectionalLight(){}