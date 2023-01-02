#include<iostream>
#include<cstdlib>


//include glad before GLFW to avoid header conflict or define "#define GLFW_INCLUDE_NONE"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_inverse.hpp>


#include "camera.h"
#include "shader.h"
#include "object.h"
//#include "physics.h"
// #include "debug.h"

#include "display.h"
#include "process.h"


Display mainWindow; 

char fileVert[128] = "../../src/Shaders/vertSrc.txt";
char fileFrag[128] = "../../src/Shaders/fragSrc.txt";
char groundVertex[128] = "../../src/Shaders/vertGround.txt";
char groundFrag[128] = "../../src/Shaders/fragGround.txt";
char groundImage[128] = "../../image/woodFloor.png";


Camera camera(glm::vec3(0.0, 15.0, -25.0), glm::vec3(0.0, 1.0, 0.0), 90.0, -30.);


float getRandom(float from=-4, float to=4) {
	int mod = (to - from)*100;
	return float(rand()%mod + 100*from)/100;
}

int main(int argc, char* argv[]){
	std::cout << "Project is running... " << std::endl;

	mainWindow = Display(false); // if cursor disabled -> true, otherwise false.
	mainWindow.Initialise(); 

	Shader shader(NULL, fileVert, fileFrag, false, true);
	Shader groundShader(groundImage, groundVertex, groundFrag, true, false);

	char sphereGeometry[] = "../../objects/sphere.obj";
	char cubeGeometry[] = "../../objects/cube.obj";
	char groundGeometry[] = "../../objects/floor.obj";

	Object ground_obj = Object(groundGeometry, glm::vec3(0.), glm::vec3(0.), glm::vec3(1.), 0);
    PhysicalWorld world = PhysicalWorld(&ground_obj); // BULLET3
	groundShader.addObject(&ground_obj);

	/*
	Object sphere;
	for (int i=0; i<40; i++) {
		glm::vec3 pos = glm::vec3(getRandom(), 2.+5*i, getRandom());
		glm::vec3 rot = glm::vec3(getRandom(0.,3.14), getRandom(0.,3.14), getRandom(0.,3.14));
		glm::vec3 scale = glm::vec3(getRandom(0.5,2.));
		Object* sphere = new Object(sphereGeometry, pos, rot, scale, world.glObjects.size());	
		world.addSphere(sphere);  
		shader.addObject(sphere);
	}
	Object cube;
	for (int i=0; i<10; i++) {
		glm::vec3 pos = glm::vec3(getRandom(), 2.+5*i, getRandom());
		glm::vec3 rot = glm::vec3(getRandom(0.,3.14), getRandom(0.,3.14), getRandom(0.,3.14));
		glm::vec3 scale = glm::vec3(getRandom(0.5,2.), getRandom(0.5,2.), getRandom(0.5,2.));
		Object* cube = new Object(cubeGeometry, pos, rot, scale, world.glObjects.size());	
		world.addCube(cube);  
		shader.addObject(cube);
	}
	*/

	Object sphere = Object(sphereGeometry, glm::vec3(0., 1., 0.), glm::vec3(0., 0, 0), glm::vec3(1.), world.glObjects.size());	
	world.addSphere(&sphere);  
	shader.addObject(&sphere);
	Object cube = Object(cubeGeometry, glm::vec3(-4, 4, -4), glm::vec3(0., 0, 0), glm::vec3(1.), world.glObjects.size());	
	world.addCube(&cube);
	shader.addObject(&cube);

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



	// glm::mat4 view = camera.getViewMatrix();
	// // printf("camera value: %f", camera.ZOOM); 
	// glm::mat4 projection = camera.getProjectionMatrix(glm::radians(camera.ZOOM), mainWindow.getBufferWidth()/mainWindow.getBufferHeight(), 0.01, 100.0);



	//Rendering
	glfwSwapInterval(1);
	Process process = Process();

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // only show the vertexes

    glfwSetWindowUserPointer(mainWindow.getWindow(), reinterpret_cast<void *>(&camera));

	process.initMousePosition(mainWindow.getWindow(), camera, mainWindow.getCursorDisabled());

	while (!mainWindow.getShouldClose()){
		// BULLET3
		world.animate();

		process.processInput(mainWindow.getWindow(), camera, world, shader);

		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = camera.getProjectionMatrix(mainWindow.getWindow(), 0.01, 100.0);
		glfwPollEvents();
		double now = glfwGetTime();
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//2. Use the shader Class to send the relevant uniform*glm::vec4(object->position, 1.0)
		std::cout << shader.objectList.size() << std::endl;
		shader.DrawObjects(view, projection, light_pos);
		groundShader.DrawObjects(view, projection, light_pos);
		
		fps(now);
		mainWindow.swapBuffers(); 
	}

	// BULLET3
	world.clear();

	return 0;
}
