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
		Object* sphere = new Object(geometry::sphere,  ShaderType::LIGHT, Textures::White(), Materials::Empty(), PHYSIC::UNMOVABLE, pos, rot, scale, color);
		pointLightObjects.push_back(sphere);

		pointLights[i] = PointLight(general::pointShadowResolution, general::pointShadowResolution,
                                    0.01, 100.0f,
                                    sphere->color.x, sphere->color.y, sphere->color.z, 
                                    1.0f, 2.0,
                                    sphere->position.x, sphere->position.y, sphere->position.z,
                                    0.3f, 0.2f, 0.1f); //0.003f, 0.002f, 0.001f
    	pointLightCount++; 
    }
}

void LightConstructor::createSpotLight(){
    
    createTorch();

    Object* sphere = new Object(geometry::sphere,  ShaderType::LIGHT, Textures::White(), Materials::Empty(), PHYSIC::UNMOVABLE, 
                                glm::vec3(-10.0f, 5.0f, -10.0f), glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.05), glm::vec3(1.));
	spotLightObjects.push_back(sphere);
    spotLights[1] = SpotLight(general::spotShadowResolution, general::spotShadowResolution, 
                            0.01f, 100.0f,
                            sphere->color.x, sphere->color.y, sphere->color.z, 
                            0.0f, 2.0f,
                            sphere->position.x, sphere->position.y, sphere->position.z,
                            sphere->rotation.x, sphere->rotation.y, sphere->rotation.z,
                            0.3f, 0.2f, 0.5f, // we don't want th elight to die off because of distance
                            30.0f);  // spread of the angle : 20°
    spotLightCount++; 
}

void LightConstructor::createTorch(){
    // the first one is a torch
    Object* sphere = new Object(geometry::sphere,  ShaderType::LIGHT, Textures::White(), Materials::Empty(), PHYSIC::UNMOVABLE, 
                                glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.05), glm::vec3(1.));
	spotLightObjects.push_back(sphere);
    spotLights[0] = SpotLight(general::spotShadowResolution, general::spotShadowResolution, 
                            0.01f, 100.0f,
                            sphere->color.x, sphere->color.y, sphere->color.z, 
                            0.0f, 2.0f,
                            sphere->position.x, sphere->position.y, sphere->position.z,
                            sphere->rotation.x, sphere->rotation.y, sphere->rotation.z,
                            0.3f, 0.2f, 0.5f, // we don't want th elight to die off because of distance
                            30.0f);  // spread of the angle : 20°
    spotLightCount++;
}

void LightConstructor::createAreaLight() {
    for (int i=0; i<1; i++) {
		glm::vec3 pos = glm::vec3(Utils::getRandom(-20,20),1,Utils::getRandom(-20,20));
		glm::vec3 rot = glm::vec3(glm::radians(-90.0), Utils::getRandom(glm::radians(-90.0),glm::radians(90.0)), 0);//getRandom(glm::radians(-90.0),glm::radians(90.0)), getRandom(0.,2*3.14), 0);
		glm::vec3 scale = glm::vec3(1);
        glm::vec3 color = Utils::getRandom3(0, 1);
		Object* plane = new Object(geometry::plane, ShaderType::LIGHT, Textures::Dirt(), Materials::Empty(), PHYSIC::UNMOVABLE, pos, rot, scale, color);
        areaLightObjects.push_back(plane);


		areaLights[i] = AreaLight(plane->color.x,plane->color.y, plane->color.z, 
							  0.9f, 10.,
							  plane->getPosition(),
							  0.003f, 0.002f, 0.001f, // not used
							  plane->getRotation(), true, 
							  plane->getVertexPosition(),
							  plane->getScale());
    	areaLightCount++; 
    }
}

LightConstructor::~LightConstructor(){}