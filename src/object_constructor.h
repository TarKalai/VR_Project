#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "object.h"
#include "constant.h"
#include "utils.h"
#include "physics.h"





class ObjectConstructor{

public: 
    ObjectConstructor(PhysicalWorld *physicalWorld); 
    ~ObjectConstructor(); 
    
    // getters & setters
    std::vector<Object*> getObjects(){return objects;}


private: 
    std::vector<Object*> objects;
}; 