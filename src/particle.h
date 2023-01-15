#pragma once
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector> // dynamic arrays
#include <random> // random data to particles

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //use type_ptr to pass the values to the shaders
#include "glm/gtx/string_cast.hpp"

#include "constant.h"


class Particle
{
public: 

    Particle(); 

    void CreateParticles(GLuint numberOfParticlesIn); 

    void Render(); 

    ~Particle(); 


private: 

    GLuint particleObjectLocation;
    GLuint numberOfParticles; 

};