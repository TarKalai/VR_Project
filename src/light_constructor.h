#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "directionalLight.h"
#include "pointLight.h"
#include "spotLight.h"
#include "areaLight.h"
#include "object.h"
#include "utils.h"
#include <vector>

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
    std::vector<Object*> getPointLightObjects(){return pointLightObjects;}


private: 
    DirectionalLight mainLight;

    int pointLightCount = 0; 
    PointLight pointLights[values::MAX_POINT_LIGHTS];
    std::vector<Object*> pointLightObjects;


    int spotLightCount = 0;
    SpotLight spotLights[values::MAX_SPOT_LIGHTS];
    Object spotLightObjects[values::MAX_POINT_LIGHTS];
}; 