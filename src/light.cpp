
#include "light.h"

Light::Light(){

    color = glm::vec3(1.0f, 1.0f, 1.0f); // The color is defined by the texture, here we are telling how much of that color we want to use. Here we say we are using all the colors there will be
    ambientIntensity = 1.0f; 
    direction = glm::vec3(0.0f, -1.0f, 0.0f); 
    diffuseIntensity = 0.0f; 
}

Light::Light(GLuint shadowWidth, GLuint shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity){

    color = glm::vec3(red, green, blue); 
    ambientIntensity = aIntensity;
    diffuseIntensity = dIntensity;  

    shadowMap = new ShadowMap();
    shadowMap->Init(shadowWidth, shadowHeight);  
}

Light::~Light(){}