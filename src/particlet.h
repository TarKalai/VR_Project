#pragma once
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //use type_ptr to pass the values to the shaders
#include "glm/gtx/string_cast.hpp"

#include "constant.h"

class Particle
{
public : 

    Particle(glm::vec3 position, glm::vec3 velocity, float gravityEffect, float lifeLength, float rotation, float scale); 
    
    bool Update(); 

    glm::vec3 GetPosition(); 
    float GetRotation(); 
    float GetScale(); 

    ~Particle();



private: 

    glm::vec3 position; 
    glm::vec3 velocity; 
    float gravityEffect; //indicates how much a particle is affected by gravity.
    float lifeLength; // how long the particles stay around
    float rotation; // to make the particles rotate
    float scale;

    float elapsedTime = 0; // time since the particle has been alive.
};
