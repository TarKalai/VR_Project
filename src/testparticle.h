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
    ~Particle();

    glm::vec3 GetPosition(); 
    float GetRotation(); 
    float GetScale(); 




private: 

    glm::vec3 position; 
    glm::vec3 velocity; 
    float gravityEffect; //indicates how much a particle is affected by gravity.
    float lifeLength; // how long the particles stay around
    float rotation; // to make the particles rotate
    float scale;

    float elapsedTime = 0; // time since the particle has been alive.
};

// #include "particle.h"

Particle::Particle(glm::vec3 position, glm::vec3 velocity, float gravityEffect, float lifeLength, float rotation, float scale)
{
    this.position = position; 
    this.velocity = velocity; 
    this.gravityEffect = gravityEffect; 
    this.lifeLength = lifeLength; 
    this.rotation = rotation; 
    this.scale = scale; 

}


Particle::GetPosition()
{
    return position; 
}

Particle::GetRotation()
{
    return rotation; 
}

Particle::GetScale(){
    return scale; 
}

bool Particle::Update() 
// return false if the particle has been around 
// longer than its lifelength which means it has to be removed
{
    velocity.y += general::gravity * gravityEffect * 1/60; 
    glm::vec3 change = velocity; 
    change.sca
}

Particle::Particle()
{
}
Particle::~Particle() {}