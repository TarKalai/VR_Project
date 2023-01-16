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
    for (int i=0; i<10; i++) {
		glm::vec3 pos = glm::vec3(Utils::getRandom(-30.0, 30.0),Utils::getRandom(1.0, 5.0),Utils::getRandom(-30.0, 30.0));
		glm::vec3 rot = glm::vec3(0.0, 0.0, 0.0);
		glm::vec3 scale = glm::vec3(0.05);
        glm::vec3 color = Utils::getRandom3();
		Object* point = new Object(geometry::sphere,  ShaderType::POINTLIGHT, Textures::White(), Materials::Empty(), PHYSIC::UNMOVABLE, pos, rot, scale, color);
		addPointLight(point);
    }
}


void LightConstructor::createSpotLight(){
    
    Object* torch = new Object(geometry::cube,  ShaderType::SPOTLIGHT, Textures::White(), Materials::Empty(), PHYSIC::UNMOVABLE, 
                                glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.05), glm::vec3(1.));
    Object* spot1 = new Object(geometry::cube,  ShaderType::SPOTLIGHT, Textures::White(), Materials::Empty(), PHYSIC::UNMOVABLE, 
                                glm::vec3(-10.0f, 5.0f, -30.0f), glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.05), glm::vec3(1.,0.,0.));
	
    addSpotLight(torch);
    addSpotLight(spot1); 
}


void LightConstructor::createAreaLight() {
    for (int i=0; i<10; i++) {
		glm::vec3 pos = glm::vec3(Utils::getRandom(-20,20),1,Utils::getRandom(-20,20));
		glm::vec3 rot = glm::vec3(glm::radians(-90.0), Utils::getRandom(glm::radians(-90.0),glm::radians(90.0)), 0);//getRandom(glm::radians(-90.0),glm::radians(90.0)), getRandom(0.,2*3.14), 0);
		glm::vec3 scale = glm::vec3(1);
        glm::vec3 color = Utils::getRandom3(0, 1);
		Object* area = new Object(geometry::plane, ShaderType::AREALIGHT, Textures::Dirt(), Materials::Empty(), PHYSIC::UNMOVABLE, pos, rot, scale, color);
        addAreaLight(area);
    }
}


void LightConstructor::reset() {
    pointLightCount = 0; 
    spotLightCount = 0;
    areaLightCount = 0;
    lightObjects.clear();
}

LightConstructor::~LightConstructor(){}