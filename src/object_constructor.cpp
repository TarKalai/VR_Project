#include "object_constructor.h"



ObjectConstructor::ObjectConstructor(PhysicalWorld *physicalWorld){
	
    for (int i=0; i<10; i++) {
		glm::vec3 pos = glm::vec3(Utils::getRandom(), 2.+5*i, Utils::getRandom());
		glm::vec3 rot = glm::vec3(Utils::getRandom(0.,3.14), Utils::getRandom(0.,3.14), Utils::getRandom(0.,3.14));
		glm::vec3 scale = glm::vec3(Utils::getRandom(0.5,2.));
		glm::vec3 color = glm::vec3(1.0);
		Object* sphere = new Object(geometry::sphere, Textures::Dirt(), Materials::Shiny(), pos, rot, scale, color);
        objects.push_back(sphere);
		physicalWorld->addObject(sphere);
	}

	for (int i=0; i<10; i++) {
		glm::vec3 pos = glm::vec3(Utils::getRandom(), 2.+5*i, Utils::getRandom());
		glm::vec3 rot = glm::vec3(Utils::getRandom(0.,3.14), Utils::getRandom(0.,3.14), Utils::getRandom(0.,3.14));
		glm::vec3 scale = glm::vec3(Utils::getRandom(0.5,2.), Utils::getRandom(0.5,2.), Utils::getRandom(0.5,2.));
		glm::vec3 color = glm::vec3(1.0);
		Object* cube = new Object(geometry::cube, Textures::Brick(), Materials::Shiny(), pos, rot, scale, color);
        objects.push_back(cube);
		physicalWorld->addObject(cube);
	}
}

ObjectConstructor::~ObjectConstructor(){}