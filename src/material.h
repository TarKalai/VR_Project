#pragma once

#include <glad/glad.h>

class Material{
public: 

    Material(); 
    Material(GLfloat sIntensity, GLfloat shine); 

    void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation); 

    ~Material(); 

private: 

    GLfloat specularIntensity; // how much specular light will be on the object
    GLfloat shininess; // how smooth the surface is going to be rendered out, if we have a small value we would have a spread out lighting on the object (rough surface)
    // with high value all the light will be bright and intense and collected into a smaller point. 

}; 