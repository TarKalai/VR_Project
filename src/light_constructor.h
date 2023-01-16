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
    
    // getters & setters
    DirectionalLight* getMainLight(){return &mainLight;}

    int getPointLightCount(){return pointLightCount;}
    PointLight* getPointLight(){return pointLights;}
    std::vector<Object*> getPointLightObjects(){return pointLightObjects;}

    int getSpotLightCount(){return spotLightCount;}
    SpotLight* getSpotLight(){return spotLights;}
    std::vector<Object*> getSpotLightObjects(){return spotLightObjects;}

    int getAreaLightCount(){return areaLightCount;}
    AreaLight* getAreaLight(){return areaLights;}
    std::vector<Object*> getAreaLightObjects(){return areaLightObjects;}


private: 
    void createMainLight();
    void createPointLight();
    void createSpotLight();
    void createAreaLight();
    void createTorch();

    DirectionalLight mainLight;

    int pointLightCount = 0; 
    PointLight pointLights[values::MAX_POINT_LIGHTS];
    std::vector<Object*> pointLightObjects;

    int spotLightCount = 0;
    SpotLight spotLights[values::MAX_SPOT_LIGHTS];
    std::vector<Object*> spotLightObjects;

    int areaLightCount = 0;
    AreaLight areaLights[values::MAX_AREA_LIGHTS];
    std::vector<Object*> areaLightObjects;
}; 