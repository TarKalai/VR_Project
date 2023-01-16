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

    void reset();
    
    // getters
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

    // setter
    void addPointLight(Object* sphere) {
        pointLightObjects.push_back(sphere);
		pointLights[pointLightCount] = PointLight(general::pointShadowResolution, general::pointShadowResolution,
                                    0.01, 100.0f,
                                    sphere->color.x, sphere->color.y, sphere->color.z, 
                                    .0f, 0.1,
                                    sphere->position.x, sphere->position.y, sphere->position.z,
                                    0.01f, 0.005f, 0.002f); //0.003f, 0.002f, 0.001f
    	pointLightCount++; 
    }

    void addSpotLight(Object* sphere) {
        spotLightObjects.push_back(sphere);
        spotLights[spotLightCount] = SpotLight(general::spotShadowResolution, general::spotShadowResolution, 
                                0.01f, 100.0f,
                                sphere->color.x, sphere->color.y, sphere->color.z, 
                                0.0f, 1.0f,
                                sphere->position.x, sphere->position.y, sphere->position.z,
                                sphere->rotation.x, sphere->rotation.y, sphere->rotation.z,
                                0.01f, 0.005f, 0.002f, //cst, dist, dist**2
                                30.0f);  // spread of the angle : 20°
        spotLightCount++;
    }

    void addAreaLight(Object* plane) {
        areaLightObjects.push_back(plane);
		areaLights[areaLightCount] = AreaLight(plane->color.x,plane->color.y, plane->color.z, 
							  0.f, 3.,
							  plane->getPosition(),
							  0.0f, 0.00f, 0.00f, // not used
							  plane->getRotation(), true, 
							  plane->getVertexPosition(),
							  plane->getScale());
    	areaLightCount++; 
    }


private: 
    void createMainLight();
    void createPointLight();
    void createSpotLight();
    void createAreaLight();

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