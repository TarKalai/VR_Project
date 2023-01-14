#pragma once


#include <vector>
#include <string>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //use type_ptr to pass the values to the shaders


#include "constant.h"

#include "shader.h"

#include "object.h"

// #include "mesh.h"

class Skybox
{
public: 
    
    Skybox(); 

    Skybox(std::vector<std::string> faceLocations); // vector of all the locations of our faces

    void DrawSkyBox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix); 

   // void LoadFogColor(float r, float g, float b); 

    ~Skybox();

private:

    float rotation =0; 

    Shader* skyShader; 

    Object* skyMesh; 

    GLuint textureId; 
    GLuint uniformProjection, uniformView; //, uniformFogColor;  
};