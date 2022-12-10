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

const int width = 500;
const int height = 500;


GLuint compileShader(std::string shaderCode, GLenum shaderType);
GLuint compileProgram(GLuint vertexShader, GLuint fragmentShader);
void processInput(GLFWwindow* window);


#ifndef NDEBUG
void APIENTRY glDebugOutput(GLenum source,
	GLenum type,
	unsigned int id,
	GLenum severity,
	GLsizei length,
	const char* message,
	const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}
#endif

Camera camera(glm::vec3(0.0, 0.0, -6.0), glm::vec3(0.0, 1.0, 0.0), 90.0);


int main(int argc, char* argv[])
{
	std::cout << "Welcome to exercice 3: " << std::endl;
	std::cout << "Light - Diffuse Gouraud\n"
		"Implement Gouraud shading on a sphere.\n"
		"You need to :\n"
		"	- make a sphere and export it from Blender or use .obj files we give you \n"
		"	- put the light somewhere (and send the vec3 with the position to the shader)\n"
		"	- normals are UNITARY vectors, use the transform described in the slides \n";

	//Boilerplate
	//Create the OpenGL context 
	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialise GLFW \n");
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifndef NDEBUG
	//create a debug context to help with Debugging
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif


	//Create the window
	GLFWwindow* window = glfwCreateWindow(width, height, "Exercise 03", nullptr, nullptr);
	if (window == NULL)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window\n");
	}

	glfwMakeContextCurrent(window);


	//load openGL function
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::runtime_error("Failed to initialize GLAD");
	}

	glEnable(GL_DEPTH_TEST);

#ifndef NDEBUG
	int flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
#endif

	const std::string sourceV = "#version 330 core\n"
		"in vec3 position; \n"
		"in vec3 normal; \n"

		"out vec3 v_diffuse; \n"

		"uniform mat4 M; \n"
		"uniform mat4 itM; \n"
		"uniform mat4 V; \n"
		"uniform mat4 P; \n"
		"uniform vec3 u_light_pos; \n"

		//Think about which uniform you may need
		" void main(){ \n"
		"vec4 frag_coord = M*vec4(position, 1.0); \n"
		"gl_Position = P*V*frag_coord;\n"
		//3. transform correctly the normals
		"vec3 norm = vec3(itM * vec4(normal, 1.0)); \n"
		//3. use Gouraud : compute the diffuse light with the normals at the vertices
		"vec3 L = normalize(u_light_pos - frag_coord.xyz); \n" // direction from light to surface
		"float diffusion = max(0.0, dot(norm, L)); \n"
		"v_diffuse = vec3(diffusion);\n" //same component in every direction
		"}\n"; 
	const std::string sourceF = "#version 330 core\n"
		"out vec4 FragColor;"
		"precision mediump float; \n"
		"in vec3 v_diffuse; \n"

		"void main() { \n"
		"FragColor = vec4(v_diffuse, 1.0); \n"
		"} \n";

	char fileVert[128] = "../vertSrc.txt";
	char fileFrag[128] = "../fragSrc.txt";

    PhysicalWorld world = PhysicalWorld(); // BULLET3

	//Shader shader(fileVert, fileFrag);
	Shader shader(sourceV, sourceF);

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

	std::cout << sphere3.position.x << " " << sphere3.position.y << " " << sphere3.position.z << std::endl;
	std::cout << sphere2.position.x << " " << sphere2.position.y << " " << sphere2.position.z << std::endl;
	std::cout << sphere1.position.x << " " << sphere1.position.y << " " << sphere1.position.z << std::endl;
	
    
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



	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 perspective = camera.GetProjectionMatrix();


	//Rendering
	glfwSwapInterval(1);

	while (!glfwWindowShouldClose(window)) {

		// BULLET3
		world.animate();

		processInput(window);
		view = camera.GetViewMatrix();
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
		glfwSwapBuffers(window);
	}

	// BULLET3
	world.clear();


	//clean up ressource
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}


void processInput(GLFWwindow* window) {
	// Use the cameras class to change the parameters of the camera
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboardMovement(LEFT, 0.1);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboardMovement(RIGHT, 0.1);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboardMovement(FORWARD, 0.1);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboardMovement(BACKWARD, 0.1);

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera.ProcessKeyboardRotation(1, 0.0,1);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera.ProcessKeyboardRotation(-1, 0.0,1);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.ProcessKeyboardRotation(0.0, 1.0, 1);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.ProcessKeyboardRotation(0.0, -1.0, 1);
}