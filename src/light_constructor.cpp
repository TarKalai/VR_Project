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
    int nbr = 5;
    for (int i=0; i<nbr; i++) {
		// glm::vec3 pos = glm::vec3(Utils::getRandom(-15,15),Utils::getRandom(1.0, 3.0),Utils::getRandom(-15,15));
		// glm::vec3 rot = glm::vec3(glm::radians(-90.0), Utils::getRandom(glm::radians(-90.0),glm::radians(90.0)), 0);//getRandom(glm::radians(-90.0),glm::radians(90.0)), getRandom(0.,2*3.14), 0);
		// glm::vec3 scale = glm::vec3(1);
        // glm::vec3 color = Utils::getRandom3(0, 1);
        glm::vec3 pos = glm::vec3(15*glm::sin(0.5+2*3.14*i/nbr),Utils::getRandom(3.0, 4.0), 15*glm::cos(0.5+2*3.14*i/nbr));
        glm::vec3 rot = glm::vec3(2*3.14*i/nbr, 2*3.14*i/nbr, 0);
        glm::vec3 scale = glm::vec3(.5);
        glm::vec3 color = Utils::RainbowColor(1000*glfwGetTime());
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
    // addSpotLight(spot1); 
    for (int i=0; i<5; i++) {
		glm::vec3 pos = glm::vec3(Utils::getRandom(-10.0, 10.0),Utils::getRandom(1.0, 5.0),Utils::getRandom(-10.0, 10.0));
		glm::vec3 rot = glm::vec3(0.0, 0.0, 0.0);
		glm::vec3 scale = glm::vec3(0.05);
        glm::vec3 color = Utils::getRandom3();
		Object* point = new Object(geometry::sphere,  ShaderType::POINTLIGHT, Textures::White(), Materials::Empty(), PHYSIC::UNMOVABLE, pos, rot, scale, color);
		addSpotLight(point);
    }
}


void LightConstructor::createAreaLight() {
    
    int nbr = 5;
    for (int i=0; i<nbr; i++) {
		// glm::vec3 pos = glm::vec3(Utils::getRandom(-15,15),Utils::getRandom(1.0, 3.0),Utils::getRandom(-15,15));
		// glm::vec3 rot = glm::vec3(glm::radians(-90.0), Utils::getRandom(glm::radians(-90.0),glm::radians(90.0)), 0);//getRandom(glm::radians(-90.0),glm::radians(90.0)), getRandom(0.,2*3.14), 0);
		// glm::vec3 scale = glm::vec3(1);
        // glm::vec3 color = Utils::getRandom3(0, 1);
        glm::vec3 pos = glm::vec3(15*glm::sin(2*3.14*i/nbr),Utils::getRandom(3.0, 4.0), 15*glm::cos(2*3.14*i/nbr));
        glm::vec3 rot = glm::vec3(2*3.14*i/nbr, 2*3.14*i/nbr, 0);
        glm::vec3 scale = Utils::getRandom3(1, 2);
        glm::vec3 color = Utils::RainbowColor(1000*glfwGetTime());
		Object* area = new Object(geometry::plane, ShaderType::AREALIGHT, Textures::White(), Materials::Empty(), PHYSIC::UNMOVABLE, pos, rot, scale, color);
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