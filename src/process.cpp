#include "process.h"
#include "camera.h"
#include "string.h"
#include "display.h"

#include <glm/glm.hpp>
#include "glm/gtx/string_cast.hpp" // (print matrix) debug purpose

Process::Process(){}


/*
static int oldState = GL_RELEASE; 
int newState = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT); 
if (newState == GL_RELEASE && oldState == GL_PRESS) { 
	// whatever 
} 
oldState = newState;

*/
void Process::processInput(GLFWwindow* window, Camera &camera, PhysicalWorld &world, Shader &shader) {

	// Use the cameras class to change the parameters of the camera
	//3. Use the cameras class to change the parameters of the camera
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS) {
		if (fullscreen && !was_pressed) {
			glfwSetWindowSize(window, 700, 700);
			fullscreen = !fullscreen;
		}
		else if (!was_pressed) {
			glfwSetWindowSize(window, 1920, 1080);
			fullscreen = !fullscreen;
		}
		was_pressed = true;
	} else {
		was_pressed = false;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera.processKeyboardMovement(LEFT, 0.1);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera.processKeyboardMovement(RIGHT, 0.1);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.processKeyboardMovement(FORWARD, 0.1);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.processKeyboardMovement(BACKWARD, 0.1);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		camera.processKeyboardMovement(UP, 0.1);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
		camera.processKeyboardMovement(DOWN, 0.1);
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) 
		PutDominos(window, camera, world, shader);
	HandleMouse(window, camera); 
}

void Process::initMousePosition(GLFWwindow* window, Camera &camera, bool cursor_disabled){
	
	// int height, width;
	// glfwGetWindowSize(window, &width, &height);
	// camera.initRunX = width/2 - camera.Yaw*(1/camera.MouseSensitivity);
	// camera.initRunY = height/2 + camera.Pitch*(1/camera.MouseSensitivity);
	// printf("size width %d, height %d\n", width, height);
	
	
	if (cursor_disabled){
		int height, width;
		glfwGetWindowSize(window, &width, &height);
		camera.initRunX = width/2 - camera.Yaw*(1/camera.MouseSensitivity);
		camera.initRunY = height/2 + camera.Pitch*(1/camera.MouseSensitivity);
		printf("size width %d, height %d\n", width, height);
	}else{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos); 
		camera.initRunX = xpos - camera.Yaw*(1/camera.MouseSensitivity);
		camera.initRunY = ypos + camera.Pitch*(1/camera.MouseSensitivity);
	}

}


void Process::HandleMouse(GLFWwindow* window, Camera &camera){

	double xpos, ypos;
	float xoffset, yoffset;
	
	glfwGetCursorPos(window, &xpos, &ypos); 
	
	xoffset = xpos - camera.initRunX;
	yoffset = ypos - camera.initRunY; 

	if (xoffset || yoffset)
		camera.processMouseMovement(xoffset, yoffset, 1);

	camera.CreateCallBacks(window, xoffset, yoffset); 

}

void Process::PutDominos(GLFWwindow* window, Camera &camera, PhysicalWorld &world, Shader &shader){

	double xpos, ypos;
	int width, height;
	glfwGetCursorPos(window, &xpos, &ypos); 
	glfwGetWindowSize(window, &width, &height);
	xpos = (2*xpos-width)/width;
	ypos = (height-2*ypos)/height;

	float near = 0.01;
	float far = 100.;
	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 projection = camera.getProjectionMatrix(window, near, far);

	glm::vec4 farScreen = glm::vec4(xpos*far, ypos*far, far-0.015, far);
	glm::vec4 farModel = glm::inverse(projection*view)*farScreen;
	glm::vec4 farCoord = glm::vec4(farModel/farModel.w);
	std::cout << glm::to_string(farCoord) << std::endl;

	// TODO with that
	// std::cout << "cam " << glm::to_string(camera.Front) << std::endl;

	
	if (farCoord.y < 0) { // Look towards ground (not sky)

		glm::vec4 nearScreen = glm::vec4(xpos*near, ypos*near, near-0.015, near);
		glm::vec4 nearModel = glm::inverse(projection*view)*nearScreen;
		glm::vec4 nearCoord = glm::vec4(nearModel/nearModel.w);
		std::cout << glm::to_string(nearCoord) << std::endl;

		double y = 1; // size domino
		double ratio = 1 - (nearCoord.y-y)/(nearCoord.y-farCoord.y);
		double x = ratio*nearCoord.x + (1-ratio)*farCoord.x;
		double z = ratio*nearCoord.z + (1-ratio)*farCoord.z;
		
		char cubeGeometry[] = "../../objects/cube.obj";
		Object* cube = new Object(cubeGeometry, glm::vec3(x, y, z), glm::vec3(0., 0, 0), glm::vec3(1.), world.glObjects.size());	

		world.addCube(cube);
		shader.addObject(cube);
	}
}

Process::~Process(){}