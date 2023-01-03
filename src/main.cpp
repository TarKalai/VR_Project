#include<iostream>
#include<cstdlib>


//include glad before GLFW to avoid header conflict or define "#define GLFW_INCLUDE_NONE"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_inverse.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "camera.h"
#include "shader.h"
#include "object.h"

//#include "physics.h"

#include "display.h"
#include "process.h"
#include "directionalLight.h"
#include "material.h"
#include "pointLight.h"
#include "spotLight.h"

Display mainWindow; 

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

Material shinyMaterial; 
Material dullMaterial; 

char fileVert[128] = "../../src/Shaders/vertSrc.vs";
char fileFrag[128] = "../../src/Shaders/fragSrc.fs";
char groundVertex[128] = "../../src/Shaders/vertGround.vs";
char groundFrag[128] = "../../src/Shaders/fragGround.fs";

char areaLightVertex[128] = "../../src/Shaders/area_light.vs";
char areaLightFrag[128] = "../../src/Shaders/area_light.fs";

char lightPlaneVertex[128] = "../../src/Shaders/light_plane.vs";
char lightPlaneFrag[128] = "../../src/Shaders/light_plane.fs";

char groundImage[128] = "../../image/woodFloor.png";
Camera camera(glm::vec3(0.0, 15.0, -25.0), glm::vec3(0.0, 1.0, 0.0), 90.0, -30.);


float getRandom(float from=-4, float to=4) {
	int mod = (to - from)*100;
	return float(rand()%mod + 100*from)/100;
}

int main(int argc, char* argv[]){
	std::cout << "Project is running... " << std::endl;
	Assimp::Importer importer;
	shinyMaterial = Material(1.0f, 32); 
    dullMaterial = Material(0.3f, 4); 

    mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 
                                0.2f, 0.2f, 
                                0.0f, -1.0f, 0.0f); // direction of the light

	unsigned int pointLightCount =0; 
    
    pointLights[0] = PointLight(0.0f, 0.0f, 1.0f, 
                                0.4f, 1.0f,
                                10.0f,4.0f, 10.0f,
                                0.3f, 0.2f, 0.1f);
    pointLightCount++; 
    
    pointLights[1] = PointLight(0.0f, 1.0f, 0.0f, 
                                0.4f, 1.0f,
                                -10.0f,5.0f, 10.0f,
                                0.3f, 0.1f, 0.1f);

    pointLightCount++; 

    
    pointLights[2] = PointLight(1.0f, 0.0f, 0.0f, 
                                0.4f, 1.0f,
                                -10.0f,5.0f, -10.0f,
                                0.3f, 0.2f, 0.1f);

    pointLightCount++;

	pointLights[3] = PointLight(1.0f, 0.0f, 1.0f, 
                                0.4f, 1.0f,
                                10.0f,5.0f, -10.0f,
                                0.3f, 0.2f, 0.1f);

    pointLightCount++;

	unsigned int spotLightCount = 0;

    spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f, 
                                0.0f, 1.0f,
                                0.0f,0.0f, 0.0f, // not important for the first spotlight as this one is attached to the camera to act as a flash light.
                                0.0f, -1.0f, 0.0f, // points straight down
                                0.1f, 0.1f, 0.1f, //strenght/a*distance**2 + b*distance + c
                                20.0f);  // spread of the angle : 20°
    spotLightCount++;

    spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f, 
                                0.0f, 2.0f,
                                0.0f, 10.0f, 0.0f,
                                0.0f, -1.0f, 0.0f, // point to teh left (very far)
                                0.1f, 0.1f, 0.1f, // we don't want th elight to die off because of distance
                                30.0f);  // spread of the angle : 20°
    spotLightCount++; 

	GLuint uniformProjection = 0, uniformModel=0, uniformView=0, uniformEyePosition = 0,
    uniformSpecularIntensity=0, uniformShininess=0; 

	mainWindow = Display(true); // if cursor disabled -> true, otherwise false.

	mainWindow.Initialise(); 

	Shader shader(NULL, fileVert, fileFrag, false, true);
	Shader groundShader(groundImage, groundVertex, groundFrag, true, true);
	Shader lightShader(NULL, lightPlaneVertex,lightPlaneFrag, false, false);

	char sphereGeometry[] = "../../objects/sphere.obj";
	char cubeGeometry[] = "../../objects/cube.obj";
	char groundGeometry[] = "../../objects/floor.obj";
	char planeGeometry[] = "../../objects/plane.obj";

	Object ground_obj = Object(groundGeometry, glm::vec3(0.), glm::vec3(0.), glm::vec3(1.), 0);
    PhysicalWorld world = PhysicalWorld(&ground_obj); // BULLET3
	groundShader.addObject(&ground_obj);

	// Object sphere1 = Object(sphereGeometry, glm::vec3(4.0, 0.0, 4.0), glm::vec3(0.), glm::vec3(1.), 1);
	// world.addSphere(&sphere1);  
	// shader.addObject(&sphere1);
	std::vector<Object*> lightObjects;

	Object sphere;
	for (int i=0; i<10; i++) {
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

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	Object plane = Object(planeGeometry, glm::vec3(5., 5., 5.), glm::vec3(glm::radians(-90.0), 0, 0), glm::vec3(1.), 0);
	lightObjects.push_back(&plane);
	lightShader.addObject(&plane);
	/*
	Object sphere = Object(sphereGeometry, glm::vec3(0., 1., 0.), glm::vec3(0., 0, 0), glm::vec3(1.), world.glObjects.size());	
	world.addSphere(&sphere);  
	shader.addObject(&sphere);
	Object cube = Object(cubeGeometry, glm::vec3(-4, 4, -4), glm::vec3(0., 0, 0), glm::vec3(1.), world.glObjects.size());	
	world.addCube(&cube);
	shader.addObject(&cube);
	*/

	//2. Choose a position for the light
	// const glm::vec3 light_pos = glm::vec3(0.5, 2.5, -0.7);


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
		glm::mat4 projection = camera.getProjectionMatrix(mainWindow.getWindow(), 0.01, 1000.0);
		glfwPollEvents();
		double now = glfwGetTime();
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//2. Use the shader Class to send the relevant uniform
		shader.DrawObjects(view, projection, camera.Position, camera.Front, &mainLight, uniformSpecularIntensity, uniformShininess, pointLights, pointLightCount, spotLights, spotLightCount);
		groundShader.DrawObjects(view, projection, camera.Position, camera.Front, &mainLight, uniformSpecularIntensity, uniformShininess, pointLights, pointLightCount, spotLights, spotLightCount);
		lightShader.DrawLightObjects(view, projection);
		
		fps(now);
		mainWindow.swapBuffers(); 
	}

	// BULLET3
	world.clear();

	return 0;
}