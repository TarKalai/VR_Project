#include "particle.h"

Particle::Particle(glm::vec3 position, glm::vec3 velocity, float gravityEffect, float lifeLength, float rotation, float scale)
{
    this->position = position; 
    this->velocity = velocity; 
    this->gravityEffect = gravityEffect; 
    this->lifeLength = lifeLength; 
    this->rotation = rotation; 
    this->scale = scale; 

}


glm::vec3 Particle::GetPosition()
{
    return position; 
}

float Particle::GetRotation()
{
    return rotation; 
}

float Particle::GetScale(){
    return scale; 
}

bool Particle::Update() 
// return false if the particle has been around 
// longer than its lifelength which means it has to be removed
{
    velocity.y += general::gravity * gravityEffect * 1/60; 
    glm::vec3 change = velocity; 
    change = glm::vec3(glfwGetTime()); 
    position += change; 
    elapsedTime += glfwGetTime(); 
    return elapsedTime < lifeLength;  
    // change.sca
}

// Particle::Particle()
// {
// }
Particle::~Particle() {}