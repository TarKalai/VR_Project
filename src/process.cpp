#include "process.h"
#include "camera.h"
#include "string.h"
#include "display.h"

Process::Process(){}

void Process::processInput(GLFWwindow* window, Camera &camera) {

	// Use the cameras class to change the parameters of the camera
	//3. Use the cameras class to change the parameters of the camera
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

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
		PutDominos(window, camera);
	HandleMouse(window, camera); 


	
}

void Process::initMousePosition(GLFWwindow* window, Camera &camera, bool cursor_disabled){
	int height, width;
	glfwGetWindowSize(window, &width, &height);
	camera.initRunX = width/2 - camera.Yaw*(1/camera.MouseSensitivity);
	camera.initRunY = height/2 + camera.Pitch*(1/camera.MouseSensitivity);
	printf("size width %d, height %d\n", width, height);
	/* TODO Tarik, this separation is really needed ?
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
	*/
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

void Process::PutDominos(GLFWwindow* window, Camera &camera){

	double xpos, ypos;
	float xoffset, yoffset;
	
	glfwGetCursorPos(window, &xpos, &ypos); 
	
	xoffset = xpos + camera.initRunX;
	yoffset = ypos + camera.initRunY; 

	std::cout << "X " << xoffset << " = " << xpos << " + " << camera.initRunX << std::endl;
	std::cout << "Y " << yoffset << " = " << ypos << " + " << camera.initRunY << std::endl;

}

Process::~Process(){}