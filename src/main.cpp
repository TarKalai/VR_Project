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


// FUNCTION PROTOTYPES
// void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
// void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// void do_movement(GLfloat deltaTime);
unsigned int loadTexture(const char *path, bool gammaCorrection);
GLuint loadMTexture();
GLuint loadLUTTexture();
void renderQuad();
void renderCube();

Display mainWindow; 

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
AreaLight areaLights[MAX_AREA_LIGHTS];

Material shinyMaterial; 
Material dullMaterial; 

// char fileVert[128] = "../../src/Shaders/vertSrc.vs";
// char fileFrag[128] = "../../src/Shaders/fragSrc.fs";

char groundVertex[128] = "../../src/Shaders/vertGround.vs";
char groundFrag[128] = "../../src/Shaders/fragGround.fs";

// char areaLightVertex[128] = "../../src/Shaders/area_light.vs";
// char areaLightFrag[128] = "../../src/Shaders/area_light.fs";

char lightPlaneVertex[128] = "../../src/Shaders/light_plane.vs";
char lightPlaneFrag[128] = "../../src/Shaders/light_plane.fs";

// image paths

// char defaultImage[128] = "../../image/plain.png"; // if the object has no texture

Camera camera(glm::vec3(0.0, 15.0, -25.0), glm::vec3(0.0, 1.0, 0.0), 90.0, -30.);

struct LTC_matrices {
	GLuint mat1;
	GLuint mat2;
};

float getRandom(float from=-4, float to=4) {
	int mod = (to - from)*100;
	return float(rand()%mod + 100*from)/100;
}

GLuint loadMTexture()
{
	GLuint texture = 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64,
	             0, GL_RGBA, GL_FLOAT, LTC1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

GLuint loadLUTTexture()
{
	GLuint texture = 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64,
	             0, GL_RGBA, GL_FLOAT, LTC2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}


int main(int argc, char* argv[]){

	//mainWindow = Display(true); // if cursor disabled -> true, otherwise false.
	//mainWindow.Initialise(); 

	std::cout << "Project is running... " << std::endl;

	shinyMaterial = Material(1.f, 32.0f); 
    dullMaterial = Material(0.3f, 4); 

    mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 
                                0.05f, 0.05f, 
                                -1.0f, -1.0f, 0.0f); // direction of the light

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

	// LUT textures
    LTC_matrices mLTC;
    mLTC.mat1 = loadMTexture();
    mLTC.mat2 = loadLUTTexture();

	

	// Shader shader(NULL, fileVert, fileFrag, true, true);
	// Shader shader(defaultImage, groundVertex, groundFrag, true, true);
	Shader groundShader(groundVertex, groundFrag, true, true);
	Shader lightShader(lightPlaneVertex,lightPlaneFrag, false, false);
	Shader2D shader2D(true);

	char sphereGeometry[] = "../../objects/sphere.obj";
	char cubeGeometry[] = "../../objects/cube.obj";
	// char groundGeometry[] = "../../objects/plane.obj";
	char planeGeometry[] = "../../objects/plane.obj";

	Object ground_obj = Object(planeGeometry, glm::vec3(0.), glm::vec3(0.), glm::vec3(50., 20., 50.), 1);
    PhysicalWorld world = PhysicalWorld(&ground_obj); // BULLET3
	groundShader.addObject(&ground_obj, image::groundImage);

	// Object sphere1 = Object(sphereGeometry, glm::vec3(4.0, 0.0, 4.0), glm::vec3(0.), glm::vec3(1.), 1);
	// world.addSphere(&sphere1);  
	// shader.addObject(&sphere1);
	std::vector<Object*> lightObjects;

	Object sphere;
	for (int i=0; i<10; i++) {
		glm::vec3 pos = glm::vec3(getRandom(), 2.+5*i, getRandom());
		glm::vec3 rot = glm::vec3(getRandom(0.,3.14), getRandom(0.,3.14), getRandom(0.,3.14));
		glm::vec3 scale = glm::vec3(getRandom(0.5,2.));
		Object* sphere = new Object(sphereGeometry, pos, rot, scale);
		world.addSphere(sphere);  
		groundShader.addObject(sphere, image::damierImage);
	}
	Object cube;
	for (int i=0; i<10; i++) {
		glm::vec3 pos = glm::vec3(getRandom(), 2.+5*i, getRandom());
		glm::vec3 rot = glm::vec3(getRandom(0.,3.14), getRandom(0.,3.14), getRandom(0.,3.14));
		glm::vec3 scale = glm::vec3(getRandom(0.5,2.), getRandom(0.5,2.), getRandom(0.5,2.));
		Object* cube = new Object(cubeGeometry, pos, rot, scale);	
		world.addCube(cube);  
		groundShader.addObject(cube, image::concreteImage);
	}

	unsigned int areaLightCount =0; 

	Object plane;

	for (int i=0; i<10; i++) {
		// std::sin(glfwGetTime());
		glm::vec3 pos = glm::vec3(getRandom(-50.0, 50.0), 1., getRandom(-50.0, 50.0));
		glm::vec3 rot = glm::vec3(glm::radians(-90.0),0,0);//getRandom(glm::radians(-90.0),glm::radians(90.0)), getRandom(0.,2*3.14), 0);
		glm::vec3 scale = glm::vec3(1.0);

		Object* plane = new Object(planeGeometry, pos, rot, scale, lightObjects.size());
		lightObjects.push_back(plane);
		lightShader.addObject(plane, image::defaultImage);

		areaLights[i] = AreaLight(getRandom(0.0, 1.0), getRandom(0.0, 1.0), getRandom(0.0, 1.0), 
							  0.4f, 1.0f,
							  plane->getPosition(),
							  0.3f, 0.2f, 0.1f, // not used
							  plane->getRotation(), true, 
							  plane->getVertexPosition());
    	areaLightCount++; 

	}
	
	//Rendering
	glfwSwapInterval(1);
	Process process = Process(&mainWindow, &camera, &world, &groundShader);

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // only show the vertexes

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
		
		//2. Use the shader Class to send the relevant uniform
		// shader.DrawObjects(view, projection, camera.Position, camera.Front, &mainLight, uniformSpecularIntensity, uniformShininess, pointLights, pointLightCount, spotLights, spotLightCount, areaLights, areaLightCount, shinyMaterial);
		groundShader.DrawObjects(view, projection, camera.Position, camera.Front, &mainLight, uniformSpecularIntensity, uniformShininess, pointLights, pointLightCount, spotLights, spotLightCount, areaLights, areaLightCount, shinyMaterial);
		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D, mLTC.mat1);
		// glActiveTexture(GL_TEXTURE1);
		// glBindTexture(GL_TEXTURE_2D, mLTC.mat2);
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