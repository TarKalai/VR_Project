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


#include "physics.h"
#include "display.h"
#include "gui.h"
#include "process.h"
#include "directionalLight.h"
#include "material.h"
#include "pointLight.h"
#include "spotLight.h"
#include "areaLight.h"
#include "constant.h"
#include "light_constructor.h"
#include "object_constructor.h"
#include "utils.h"


Display mainWindow; 
AreaLight areaLights[values::MAX_AREA_LIGHTS];

Material shinyMaterial; 
Material dullMaterial; 

Camera camera(glm::vec3(0.0, 15.0, -25.0), glm::vec3(0.0, 1.0, 0.0), 90.0, -30.);

int main(int argc, char* argv[]){
	std::cout << "Project is running... " << std::endl;

	mainWindow = Display(true); // if cursor disabled -> true, otherwise false.
	mainWindow.Initialise();

	Shader groundShader(shaderfiles::groundVertex, shaderfiles::groundFrag, true, true);
	Shader lightShader(shaderfiles::lightPlaneVertex, shaderfiles::lightPlaneFrag, false, false);
	Shader2D shader2D(true);

	Object ground_obj = Object(geometry::plane, image::dirt, glm::vec3(0.), glm::vec3(0.), glm::vec3(50., 20., 50.), 1);
    PhysicalWorld world = PhysicalWorld(&ground_obj); // BULLET3
	groundShader.addObject(&ground_obj);

	ObjectConstructor objectConstructor = ObjectConstructor(&world);
	groundShader.addObjects(objectConstructor.getObjects());

	shinyMaterial = Material(0.f, 0.0f); 
    dullMaterial = Material(0.3f, 4); 

	LightConstructor lightConstructor = LightConstructor();

	// final constructor of lightobjects TODO !
	unsigned int areaLightCount =0; 


	for (int i=0; i<1; i++) {
		// std::sin(glfwGetTime());
		glm::vec3 pos = glm::vec3(Utils::getRandom(-50.0, 50.0), 0., Utils::getRandom(-50.0, 50.0));
		glm::vec3 rot = glm::vec3(glm::radians(-90.0), 0, 0);//getRandom(glm::radians(-90.0),glm::radians(90.0)), getRandom(0.,2*3.14), 0);
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
		glm::mat4 projection = camera.getProjectionMatrix(mainWindow.getWindow(), 0.01, 200.0);
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