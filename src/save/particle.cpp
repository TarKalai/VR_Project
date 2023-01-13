#include "particle.h"

Particle::Particle(){}

void Particle::CreateParticles(GLuint numberOfParticlesIn)
{
    numberOfParticles = numberOfParticlesIn; 
    std::vector<GLfloat> particlePositionData; 
    GLfloat minimiser = 2.0f/numberOfParticles; 

    std::default_random_engine generator; 
    std::uniform_int_distribution<int> distribution(-(int)(numberOfParticles), numberOfParticles); 

    for (size_t i = 0; i < numberOfParticles; i++)
    {
        particlePositionData.push_back(-1.0f + i * minimiser); // x position
        particlePositionData.push_back(sin(i * minimiser * 2) + (GLfloat)distribution(generator) * minimiser / 20); // y position
    }

    glGenVertexArrays(1, &particleObjectLocation); 
    glBindVertexArray(particleObjectLocation);

    GLuint particlePositionBufferLocation; 
    glGenBuffers(1, &particlePositionBufferLocation); 
    glBindBuffer(GL_ARRAY_BUFFER, particlePositionBufferLocation); 
    glBufferData(GL_ARRAY_BUFFER, particlePositionData.size() * sizeof(GLfloat), particlePositionData.data(), GL_STATIC_DRAW);


    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexArrayAttrib(particleObjectLocation, 0); 

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
}

void Particle::Render()
{
    glBindVertexArray(particleObjectLocation); 
    // particleObjectLocation is in the graphics card memory and with glBindVertexArray we tell it to draw
    glDrawArrays(GL_POINTS, 0, numberOfParticles); 

    glBindVertexArray(0); 

}

Particle::~Particle(){}