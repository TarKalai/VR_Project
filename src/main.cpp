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

//#include "physics.h"

#include "display.h"
#include "process.h"
#include "directionalLight.h"
#include "material.h"
#include "pointLight.h"
#include "spotLight.h"
#include "shadowMap.h"
#include "texture.h"

// Shader shader; 
Display mainWindow; 

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

Texture brickTexture; 
Texture dirtTexture; 
Texture plainTexture;

Material shinyMaterial; 
Material dullMaterial; 

GLuint uniformProjection = 0, uniformModel=0, uniformView=0, uniformEyePosition = 0,
    uniformSpecularIntensity=0, uniformShininess=0; 

char fileVert[128] = "../../src/Shaders/shader.vert";
char fileFrag[128] = "../../src/Shaders/shader.frag";
char groundVertex[128] = "../../src/Shaders/ground_shader.vert";
char groundFrag[128] = "../../src/Shaders/ground_shader.frag";
char groundImage[128] = "../../image/woodFloor.png";
char defaultImage[128] = "../../image/plain.png";

char directionalShadowVert[128] = "../../src/Shaders/directional_shadow_map.vert";
char directionalShadowFrag[128] = "../../src/Shaders/directional_shadow_map.frag"; 




float getRandom(float from=-4, float to=4) {
	int mod = (to - from)*100;
	return float(rand()%mod + 100*from)/100;
}

int main(int argc, char* argv[]){
	std::cout << "Project is running... " << std::endl;

	mainWindow = Display(true); // if cursor disabled -> true, otherwise false.

	mainWindow.Initialise(); 

	Shader shader(defaultImage, groundVertex, groundFrag, true, true);
	Shader groundShader(groundImage, groundVertex, groundFrag, true, true);
	Shader2D shader2D(true);
	Shader directionalShadowShader(defaultImage, directionalShadowVert, directionalShadowFrag, false, false); 

	Camera camera(glm::vec3(0.0, 20.0, -25.0), glm::vec3(0.0, 1.0, 0.0), 90.0, -30.);

	char sphereGeometry[] = "../../objects/sphere.obj";
	char cubeGeometry[] = "../../objects/cube.obj";
	char groundGeometry[] = "../../objects/plane.obj";
	// Object ground_obj = Object(groundGeometry, glm::vec3(0., 0., 0.), glm::vec3(0.), glm::vec3(20., 20., 20.));
	Object ground_obj = Object(groundGeometry, glm::vec3(0.), glm::vec3(0.), glm::vec3(1.));
	
    PhysicalWorld world = PhysicalWorld(&ground_obj); // BULLET3
	directionalShadowShader.addObject(&ground_obj, true); 
	groundShader.addObject(&ground_obj);

	Object sphere1 = Object(sphereGeometry, glm::vec3(0.0, 5.0, 0.0), glm::vec3(0.), glm::vec3(1.));
	world.addSphere(&sphere1); 
	directionalShadowShader.addObject(&sphere1, true); 
	shader.addObject(&sphere1);

	// Object cube; 
	for (int i=0; i<10; i++) {
		glm::vec3 pos = glm::vec3(getRandom(), 2.+5*i, getRandom());
		glm::vec3 rot = glm::vec3(getRandom(0.,3.14), getRandom(0.,3.14), getRandom(0.,3.14));
		glm::vec3 scale = glm::vec3(getRandom(0.5,2.), getRandom(0.5,2.), getRandom(0.5,2.));
		Object* cube = new Object(cubeGeometry, pos, rot, scale);	
		world.addCube(cube);  
		directionalShadowShader.addObject(cube, true); 
		shader.addObject(cube);
	}

	brickTexture = Texture("../../image/brick.png"); 
    brickTexture.LoadTextureA(); 

    dirtTexture = Texture("../../image/dirt.png"); 
    dirtTexture.LoadTextureA(); 

    plainTexture = Texture("../../image/plain.png");
	plainTexture.LoadTextureA();

	shinyMaterial = Material(1.0f, 32); 
    dullMaterial = Material(0.3f, 4); 

	mainLight = DirectionalLight(2048, 2048, // resolution of the shadow map !!! it is impacting the look of the shadows if we do GL_NEAREST instead of GL_LINEAR
							1.0f, 1.0f, 1.0f, 
							0.3f, 0.6f, 
							0.0f, -5.0f, -5.0f); // direction of the light

	unsigned int pointLightCount =0; 
    
    pointLights[0] = PointLight(0.0f, 0.0f, 1.0f, 
                                0.3f, 1.0f,
                                4.0f,4.0f, 4.0f,
                                0.3f, 0.2f, 0.1f);
    pointLightCount++; 
    
    pointLights[1] = PointLight(0.0f, 1.0f, 0.0f, 
                                0.4f, 1.0f,
                                0.0f,0.0f, 50.0f,
                                0.3f, 0.1f, 0.1f);

    pointLightCount++; 

    
    pointLights[2] = PointLight(1.0f, 0.0f, 0.0f, 
                                0.4f, 1.0f,
                                -50.0f,5.0f, -10.0f,
                                0.3f, 0.2f, 0.1f);

    pointLightCount++;

	pointLights[3] = PointLight(1.0f, 0.0f, 1.0f, 
                                0.4f, 1.0f,
                                50.0f,5.0f, -100.0f,
                                0.3f, 0.2f, 0.1f);

    pointLightCount++;
	// pointLightCount = 0;

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
                                4.0f, 10.0f, 4.0f,
                                0.0f, -1.0f, 0.0f, // point to teh left (very far)
                                0.1f, 0.1f, 0.1f, // we don't want th elight to die off because of distance
                                30.0f);  // spread of the angle : 20°
    spotLightCount++; 
    // spotLightCount=0; 

	
	/* Example how to create objects 
	Object sphere;
	for (int i=0; i<100; i++) {
		glm::vec3 pos = glm::vec3(getRandom(), 2.+5*i, getRandom());
		glm::vec3 rot = glm::vec3(getRandom(0.,3.14), getRandom(0.,3.14), getRandom(0.,3.14));
		glm::vec3 scale = glm::vec3(getRandom(0.5,2.));
		Object* sphere = new Object(sphereGeometry, pos, rot, scale);
		world.addSphere(sphere);  
		shader.addObject(sphere);
	}
	Object cube;
	
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

    // glm::mat4 projection = glm::perspective(glm::radians(45.0f),(GLfloat)mainWindow.getBufferWidth()/ mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Rendering

	// glfwSwapInterval(1);
	Process process = Process(mainWindow, &camera, &world, &shader);

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // only show the vertexes

    glfwSetWindowUserPointer(mainWindow.getWindow(), reinterpret_cast<void *>(&camera));

	process.initMousePosition();

	while (!mainWindow.getShouldClose()){

		glfwPollEvents(); //

		process.processInput(); //

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// process.processInput();
		// glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// glm::mat4 projection = camera.getProjectionMatrix(mainWindow.getWindow(), 0.01, 100.0);
		// glm::mat4 projection = camera.getProjectionMatrix(glm::radians(camera.ZOOM), mainWindow.getBufferWidth()/mainWindow.getBufferHeight(), 0.01, 100.0);
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),(GLfloat)mainWindow.getBufferWidth()/ mainWindow.getBufferHeight(), 0.1f, 100.0f);
		glm::mat4 view = camera.getViewMatrix();

		// glfwPollEvents();
		double now = glfwGetTime();
	

		// BULLET3
		world.animate();
		//2. Use the shader Class to send the relevant uniform

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		directionalShadowShader.DirectionalShadowMapPass(&mainLight); 

		shader.DrawObjects(view, projection, camera.Position, camera.Front, mainLight, uniformSpecularIntensity, uniformShininess, pointLights, pointLightCount, spotLights, spotLightCount);
	
		
		groundShader.DrawObjects(view, projection, camera.Position, camera.Front, mainLight, uniformSpecularIntensity, uniformShininess, pointLights, pointLightCount, spotLights, spotLightCount);	
		
		shader2D.drawObject();

	

		fps(now);

		ImGui::Begin("My name is window");
		ImGui::Text("Hello world");
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		mainWindow.swapBuffers(); 

	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// BULLET3
	world.clear();

	return 0;
}