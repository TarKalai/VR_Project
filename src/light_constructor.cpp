
#include "light_constructor.h"

LightConstructor::LightConstructor(){
    createMainLight();
    createPointLight();
    createSpotLight();
}

void LightConstructor::createMainLight(){
    mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 
                                 0.1f, 0.1f, 
                                -1.0f, -1.0f, 0.0f); 
}

void LightConstructor::createPointLight(){    
    pointLights[0] = PointLight(0.0f, 0.0f, 1.0f, 
                                0.4f, 1.0f,
                                10.0f,4.0f, 10.0f,
                                0.3f, 0.2f, 0.1f);
    pointLightCount++; 
    
    pointLights[1] = PointLight(0.0f, 1.0f, 0.0f, 
                                0.4f, 1.0f,
                                -10.0f,5.0f, 10.0f,
                                0.3f, 0.1f, 0.1f);

    pointLightCount++; 

    
    pointLights[2] = PointLight(1.0f, 0.0f, 0.0f, 
                                0.4f, 1.0f,
                                -10.0f,5.0f, -10.0f,
                                0.3f, 0.2f, 0.1f);

    pointLightCount++;

	pointLights[3] = PointLight(1.0f, 0.0f, 1.0f, 
                                0.4f, 1.0f,
                                10.0f,5.0f, -10.0f,
                                0.3f, 0.2f, 0.1f);

    pointLightCount++;
}

void LightConstructor::createSpotLight(){
    // the first one is a torch
    spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f, 
                                0.0f, 1.0f,
                                0.0f,0.0f, 0.0f, // not important for the first spotlight as this one is attached to the camera to act as a flash light.
                                0.0f, -1.0f, 0.0f, // points straight down
                                0.1f, 0.1f, 0.1f, //strenght/a*distance**2 + b*distance + c
                                20.0f);  // spread of the angle : 20°
    spotLightCount++;

    spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f, 
                                0.0f, 2.0f,
                                0.0f, 10.0f, 0.0f,
                                0.0f, -1.0f, 0.0f, // point to teh left (very far)
                                0.1f, 0.1f, 0.1f, // we don't want th elight to die off because of distance
                                30.0f);  // spread of the angle : 20°
    spotLightCount++; 
}



LightConstructor::~LightConstructor(){}