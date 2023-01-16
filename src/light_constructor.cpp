#include "light_constructor.h"

LightConstructor::LightConstructor(){
    createMainLight();
    createPointLight();
    createSpotLight();
    createAreaLight();
}

void LightConstructor::createMainLight(){

    mainLight = DirectionalLight(general::mainShadowResolution, general::mainShadowResolution, 
                                1.0f, 1.0f, 1.0f, 
                                0.4f, 0.4f, 
                                -1.0f, -1.0f, 0.0f); 
}

void LightConstructor::createPointLight(){  
    for (int i=0; i<1; i++) {
		glm::vec3 pos = glm::vec3(Utils::getRandom(-30.0, 30.0),Utils::getRandom(1.0, 5.0),Utils::getRandom(-30.0, 30.0));
		glm::vec3 rot = glm::vec3(0.0, 0.0, 0.0);
		glm::vec3 scale = glm::vec3(0.25);
        glm::vec3 color = glm::vec3(0.0);
        if (i<3)
            color = color::Red;
        else if (i<7)
            color = color::Green;
        else
            color = color::Blue;
		Object* sphere = new Object(geometry::sphere,  ShaderType::POINTLIGHT, Textures::White(), Materials::Empty(), PHYSIC::UNMOVABLE, pos, rot, scale, color);
		addPointLight(sphere);
    }
}

void LightConstructor::createSpotLight(){
    
    createTorch();

    Object* sphere = new Object(geometry::sphere,  ShaderType::SPOTLIGHT, Textures::White(), Materials::Empty(), PHYSIC::UNMOVABLE, 
                                glm::vec3(-10.0f, 5.0f, -30.0f), glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.05), glm::vec3(1.,0.,0.));
	addSpotLight(sphere); 
}

void LightConstructor::createTorch(){
    // the first one is a torch
    Object* torch = new Object(geometry::sphere,  ShaderType::SPOTLIGHT, Textures::White(), Materials::Empty(), PHYSIC::UNMOVABLE, 
                                glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.05), glm::vec3(1.));
	addSpotLight(torch);
}

void LightConstructor::createAreaLight() {
    for (int i=0; i<1; i++) {
		glm::vec3 pos = glm::vec3(Utils::getRandom(-20,20),1,Utils::getRandom(-20,20));
		glm::vec3 rot = glm::vec3(glm::radians(-90.0), Utils::getRandom(glm::radians(-90.0),glm::radians(90.0)), 0);//getRandom(glm::radians(-90.0),glm::radians(90.0)), getRandom(0.,2*3.14), 0);
		glm::vec3 scale = glm::vec3(1);
        glm::vec3 color = Utils::getRandom3(0, 1);
		Object* plane = new Object(geometry::plane, ShaderType::AREALIGHT, Textures::Dirt(), Materials::Empty(), PHYSIC::UNMOVABLE, pos, rot, scale, color);
        addAreaLight(plane);
    }
}

void LightConstructor::reset() {
    pointLightCount = 0; 
    pointLightObjects.clear();

    spotLightCount = 0;
    spotLightObjects.clear();

    areaLightCount = 0;
    areaLightObjects.clear();
}

LightConstructor::~LightConstructor(){}