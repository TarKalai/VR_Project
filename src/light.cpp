#include "light.h"

Light::Light(){

    color = glm::vec3(1.0f, 1.0f, 1.0f); // The color is defined by the texture, here we are telling how much of that color we want to use. Here we say we are using all the colors there will be
    ambientIntensity = 1.0f; 
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat intensity){

    color = glm::vec3(red, green, blue); 
    ambientIntensity = intensity; 

}

void Light::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation){

    glUniform3f(ambientColorLocation, color.x, color.y, color.z); 
    glUniform1f(ambientIntensityLocation, ambientIntensity); 
}

Light::~Light(){}