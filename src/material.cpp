
#include "material.h"

Material::Material(){
    specularIntensity = 0.0f; 
    shininess = 0.0f; 
}

Material:: Material(GLfloat sIntensity, GLfloat shine, const char* _name){
    specularIntensity = sIntensity; 
    shininess = shine; 
    name = _name;
}

void Material::UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation){

    // For specular lighting we need the information on the materail but also the position of the camera because we need to know the position of the viwer

    glUniform1f(specularIntensityLocation, specularIntensity); 
    glUniform1f(shininessLocation, shininess); 
}

Material::~Material(){}