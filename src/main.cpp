#include<iostream>



//include glad before GLFW to avoid header conflict or define "#define GLFW_INCLUDE_NONE"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_inverse.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include "camera.h"
#include "shader.h"
#include "object.h"
#include "physics.h"
// #include "debug.h"

#include "display.h"
#include "process.h"


Display mainWindow; 
const GLint width = 1920, height = 1080;

char fileVert[128] = "../../src/Shaders/vertSrc.txt";
char fileFrag[128] = "../../src/Shaders/fragSrc.txt";

Camera camera(glm::vec3(-2.0, 7.0, -15.0), glm::vec3(0.0, 1.0, 0.0), 90.0);



int main(int argc, char* argv[])
{
	std::cout << "Project is running... " << std::endl;

	mainWindow = Display(width, height); 
	mainWindow.Initialise(); 

    PhysicalWorld world = PhysicalWorld(); // BULLET3
	Shader shader(fileVert, fileFrag, false, true);

	//1. Load the model for 3 types of spheres
	char sphereGeometry[] = "../../objects/sphere_smooth.obj";// BULLET3

	//Object obj;
	for (int i=0; i<1000; i++) {
		Object* obj = new Object(sphereGeometry, glm::vec3(0., 2.+3*i, 0.), 1., world.glObjects.size());	
		world.addSphere(obj);  // BULLET crée des bugs !
		shader.addObject(obj);	
	}

	
	//2. Choose a position for the light
	const glm::vec3 light_pos = glm::vec3(0.5, 2.5, -0.7);


	double prev = 0;
	int deltaFrame = 0;
	//fps function
	auto fps = [&](double now) {
		double deltaTime = now - prev;
		deltaFrame++;
		if (deltaTime > 0.5) {
			prev = now;
			const double fpsCount = (double)deltaFrame / deltaTime;
			deltaFrame = 0;
			std::cout << "\r FPS: " << fpsCount;
		}
	};



	glm::mat4 view = camera.getViewMatrix();
	// glm::mat4 perspective = camera.getProjectionMatrix(45.0, (GLfloat)width/(GLfloat)height, 0.01, 100.0);
	glm::mat4 perspective = camera.getProjectionMatrix(45.0, mainWindow.getBufferWidth()/mainWindow.getBufferHeight(), 0.01, 100.0);


	//Rendering
	glfwSwapInterval(1);
	Process process = Process();

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // only show the vertexes

	// while (!glfwWindowShouldClose(window)) {
	while (!mainWindow.getShouldClose()){

		// BULLET3
		world.animate();

		process.processInput(mainWindow.getWindow(), camera);
		// processInput(window);
		view = camera.getViewMatrix();
		glfwPollEvents();
		double now = glfwGetTime();
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//2. Use the shader Class to send the relevant uniform
		shader.DrawObjects(view, perspective, light_pos);
		
		fps(now);
		mainWindow.swapBuffers(); 
		// glfwSwapBuffers(window);
	}

	// BULLET3
	world.clear();

	return 0;
}

