// #pragma once

#include<iostream>
#include<cstdlib>


//include glad before GLFW to avoid header conflict or define "#define GLFW_INCLUDE_NONE"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_inverse.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "camera.h"
#include "shader.h"
#include "shader2D.h"
#include "object.h"

// #include "physics.h"

#include "display.h"
#include "gui.h"
#include "process.h"
#include "directionalLight.h"
#include "material.h"
#include "pointLight.h"
#include "spotLight.h"
#include "areaLight.h"
#include "ltc_matrix.hpp"
#include "constant.h"
#include "light_constructor.h"
#include "utils.h"

Display mainWindow; 
AreaLight areaLights[values::MAX_AREA_LIGHTS];

Material shinyMaterial; 
Material dullMaterial; 

Camera camera(glm::vec3(0.0, 15.0, -25.0), glm::vec3(0.0, 1.0, 0.0), 90.0, -30.);

// float getRandom(float from=-4, float to=4) {
// 	int mod = (to - from)*100;
// 	return float(rand()%mod + 100*from)/100;
// }


int main(int argc, char* argv[]){
	std::cout << "Project is running... " << std::endl;

	mainWindow = Display(false); // if cursor disabled -> true, otherwise false.
	mainWindow.Initialise();

	Shader groundShader(shaderfiles::groundVertex, shaderfiles::groundFrag, true, true);
	Shader lightShader(shaderfiles::lightPlaneVertex, shaderfiles::lightPlaneFrag, false, false);
	Shader2D shader2D(true);

	shinyMaterial = Material(1.f, 32.0f); 
    dullMaterial = Material(0.3f, 4); 

	LightConstructor lightConstructor = LightConstructor();


	Object ground_obj = Object(geometry::plane, image::ground, glm::vec3(0.), glm::vec3(0.), glm::vec3(50., 20., 50.), 1);
    PhysicalWorld world = PhysicalWorld(&ground_obj); // BULLET3
	groundShader.addObject(&ground_obj);



	for (int i=0; i<10; i++) {
		glm::vec3 pos = glm::vec3(Utils::getRandom(), 2.+5*i, Utils::getRandom());
		glm::vec3 rot = glm::vec3(Utils::getRandom(0.,3.14), Utils::getRandom(0.,3.14), Utils::getRandom(0.,3.14));
		glm::vec3 scale = glm::vec3(Utils::getRandom(0.5,2.));
		Object* sphere = new Object(geometry::sphere, image::concrete, pos, rot, scale);
		world.addSphere(sphere);  
		groundShader.addObject(sphere);
	}

	for (int i=0; i<10; i++) {
		glm::vec3 pos = glm::vec3(Utils::getRandom(), 2.+5*i, Utils::getRandom());
		glm::vec3 rot = glm::vec3(Utils::getRandom(0.,3.14), Utils::getRandom(0.,3.14), Utils::getRandom(0.,3.14));
		glm::vec3 scale = glm::vec3(Utils::getRandom(0.5,2.), Utils::getRandom(0.5,2.), Utils::getRandom(0.5,2.));
		Object* cube = new Object(geometry::cube, image::concrete, pos, rot, scale);	
		world.addCube(cube);  
		groundShader.addObject(cube);
	}


	unsigned int areaLightCount =0; 


	for (int i=0; i<10; i++) {
		// std::sin(glfwGetTime());
		glm::vec3 pos = glm::vec3(Utils::getRandom(-50.0, 50.0), 5., Utils::getRandom(-50.0, 50.0));
		glm::vec3 rot = glm::vec3(Utils::getRandom(glm::radians(-90.0), glm::radians(90.0)), Utils::getRandom(glm::radians(-90.0), glm::radians(90.0)), Utils::getRandom(glm::radians(-90.0), glm::radians(90.0)));//getRandom(glm::radians(-90.0),glm::radians(90.0)), getRandom(0.,2*3.14), 0);
		glm::vec3 scale = glm::vec3(Utils::getRandom(1.0, 3.0));

		Object* plane = new Object(geometry::plane, image::white, pos, rot, scale);
		lightShader.addObject(plane);

		areaLights[i] = AreaLight(Utils::getRandom(0.0, 1.0), Utils::getRandom(0.0, 1.0), Utils::getRandom(0.0, 1.0), 
							  0.4f, Utils::getRandom(.1, 1.0),
							  plane->getPosition(),
							  0.3f, 0.2f, 0.1f, // not used
							  plane->getRotation(), true, 
							  plane->getVertexPosition(),
							  plane->getScale());
    	areaLightCount++; 
	}
	
	//Rendering
	glfwSwapInterval(1);
	Process process = Process(&mainWindow, &camera, &world, &groundShader);
    glfwSetWindowUserPointer(mainWindow.getWindow(), reinterpret_cast<void *>(&camera));
	process.initMousePosition();
	GUI gui(&process, &mainWindow, &world, &groundShader);

	while (!mainWindow.getShouldClose()){

		process.processInput();

		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = camera.getProjectionMatrix(mainWindow.getWindow(), 0.01, 1000.0);
		glfwPollEvents();
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		world.animate();
		
		groundShader.DrawObjects(view, projection, camera.Position, camera.Front, lightConstructor.getMainLight(), lightConstructor.getPointLight(), lightConstructor.getPointLightCount(), lightConstructor.getSpotLight(), lightConstructor.getSpotLightCount(), areaLights, areaLightCount, shinyMaterial);
		lightShader.DrawLightObjects(view, projection, areaLights, areaLightCount);
		
		if (!camera.pause) {
			shader2D.drawObject();
		}

		gui.update();
		mainWindow.swapBuffers(); 
	}

	gui.clear();

	// BULLET3
	world.clear();

	return 0;
}