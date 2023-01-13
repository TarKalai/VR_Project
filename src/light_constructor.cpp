#include "light_constructor.h"

LightConstructor::LightConstructor(){
    createMainLight();
    createPointLight();
    createSpotLight();
}

void LightConstructor::createMainLight(){

    mainLight = DirectionalLight(8196, 8196, 
                                1.0f, 1.0f, 1.0f, 
                                0.4f, 0.4f, 
                                -1.0f, -1.0f, 0.0f); 
}

void LightConstructor::createPointLight(){  
    for (int i=0; i<10; i++) {
		glm::vec3 pos = glm::vec3(Utils::getRandom(-30.0, 30.0),Utils::getRandom(1.0, 5.0),Utils::getRandom(-30.0, 30.0));
		glm::vec3 rot = glm::vec3(0.0, 0.0, 0.0);
		glm::vec3 scale = glm::vec3(0.25);
        glm::vec3 color = glm::vec3(Utils::getRandom(0,1),Utils::getRandom(0,1),Utils::getRandom(0,1));

		Object* sphere = new Object(geometry::sphere, Textures::White(), Materials::Empty(), pos, rot, scale, color);
		pointLightObjects.push_back(sphere);

		pointLights[i] = PointLight(color.x,color.y, color.z, 
                                    1.0f, 2.0,
                                    pos.x,pos.y,pos.z,
                                    0.3f, 0.2f, 0.1f); //0.003f, 0.002f, 0.001f
    	pointLightCount++; 
    }
}

void LightConstructor::createSpotLight(){
    
    createTorch();

    spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f, 
                                0.0f, 2.0f,
                                0.0f, 10.0f, 0.0f,
                                0.0f, -1.0f, 0.0f, // point to teh left (very far)
                                0.1f, 0.1f, 0.1f, // we don't want th elight to die off because of distance
                                30.0f);  // spread of the angle : 20°
    spotLightCount++; 
}

void LightConstructor::createTorch(){
    // the first one is a torch
    spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f, 
                                1.0f, 10.0f,
                                0.0f,0.0f, 0.0f, // not important for the first spotlight as this one is attached to the camera to act as a flash light.
                                0.0f, -1.0f, 0.0f, // points straight down
                                0.1f, 0.1f, 0.1f, //strenght/a*distance**2 + b*distance + c
                                20.0f);  // spread of the angle : 20°
    spotLightCount++;
}



LightConstructor::~LightConstructor(){}