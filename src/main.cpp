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


// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
Display mainWindow; 
const GLint width = 1900, height = 850;

// GLuint compileShader(std::string shaderCode, GLenum shaderType);
// GLuint compileProgram(GLuint vertexShader, GLuint fragmentShader);
// void processInput(GLFWwindow* mainWindow, Camera camera);
// void processInput(GLFWwindow* window);

char fileVert[128] = "../../src/Shaders/vertSrc.txt";
char fileFrag[128] = "../../src/Shaders/fragSrc.txt";

Camera camera(glm::vec3(-2.0, 7.0, -15.0), glm::vec3(0.0, 1.0, 0.0), 90.0);



int main(int argc, char* argv[])
{
	std::cout << "Project is running... " << std::endl;

	mainWindow = Display(1900, 850); 
	mainWindow.Initialise(); 

	//Boilerplate
	//Create the OpenGL context 
	// if (!glfwInit()) {
	// 	throw std::runtime_error("Failed to initialise GLFW \n");
	// }
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	

    PhysicalWorld world = PhysicalWorld(); // BULLET3

	Shader shader(fileVert, fileFrag);

	//1. Load the model for 3 types of spheres
	char path1[] = "../../objects/sphere_extremely_coarse.obj";
	char path2[] = "../../objects/sphere_coarse.obj";
	char path3[] = "../../objects/sphere_smooth.obj";// BULLET3

	Object sphere1(path1, glm::vec3(-4.0, 10., 0.0), 1.);
	sphere1.makeObject(shader, true, true, false);
	// TODO put in object ?
	sphere1.model = glm::scale(sphere1.model, glm::vec3(sphere1.scale));
	sphere1.model = glm::translate(sphere1.model, sphere1.position);


	Object sphere2(path2, glm::vec3(0., 15., 0.), 1.);
	sphere2.makeObject(shader, true, true, false);
	sphere2.model = glm::scale(sphere2.model, glm::vec3(sphere2.scale));
	sphere2.model = glm::translate(sphere2.model, sphere2.position);

	Object sphere3(path3, glm::vec3(0.5, 10., 0.), 1.);	
	sphere3.makeObject(shader, true, true, false);
	sphere3.model = glm::scale(sphere3.model, glm::vec3(sphere3.scale));
	sphere3.model = glm::translate(sphere3.model, sphere3.position);
    
    world.addSphere(&sphere1);// BULLET3
	world.addSphere(&sphere2);
	world.addSphere(&sphere3);

	
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
		shader.use();
		shader.setMatrix4("M", sphere1.model);
		shader.setMatrix4("V", view);
		shader.setMatrix4("P", perspective);
		shader.setVector3f("u_light_pos", light_pos);
		//what other uniforms do you need to send
		glm::mat4 itM = glm::inverseTranspose(sphere1.model);
		shader.setMatrix4("itM", itM);
		//don't forget to draw your objects
		sphere1.draw();

		shader.setMatrix4("M", sphere2.model);
		shader.setMatrix4("itM", glm::inverseTranspose(sphere2.model));
		sphere2.draw();

		shader.setMatrix4("M", sphere3.model);
		shader.setMatrix4("itM", glm::inverseTranspose(sphere3.model));
		sphere3.draw();

		fps(now);
		mainWindow.swapBuffers(); 
		// glfwSwapBuffers(window);
	}

	// BULLET3
	world.clear();

	return 0;
}
