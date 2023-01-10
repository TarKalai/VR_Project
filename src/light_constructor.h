#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "directionalLight.h"
#include "pointLight.h"
#include "spotLight.h"
#include "areaLight.h"

#include "constant.h"

class LightConstructor{

public: 
    LightConstructor(); 
    ~LightConstructor(); 

    void createMainLight();
    void createPointLight();
    void createSpotLight();
    void createTorch();
    
    // getters & setters
    DirectionalLight* getMainLight(){return &mainLight;}
    PointLight* getPointLight(){return pointLights;}
    SpotLight* getSpotLight(){return spotLights;}
    int getPointLightCount(){return pointLightCount;}
    int getSpotLightCount(){return spotLightCount;}


private: 
    DirectionalLight mainLight;
    int pointLightCount = 0; 
    PointLight pointLights[values::MAX_POINT_LIGHTS];
    int spotLightCount = 0;
    SpotLight spotLights[values::MAX_SPOT_LIGHTS];
}; 