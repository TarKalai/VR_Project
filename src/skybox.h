#pragma once


#include <vector>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
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

    void setDay(std::string name);
    void setNight(std::string name);

    void DrawSkyBox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix); 

    ~Skybox();

private:

    void createShader();
    void linkTexture(std::vector<std::string> faceLocations);

    void BindTextures(); 

    glm::mat4 updateRotation(glm::mat4 viewMatrix);

    GLuint textureDay, textureNight;

    Shader* skyShader; 

    Object* skyMesh; 
 
    GLuint uniformProjection, uniformView; //, uniformFogColor;  
};