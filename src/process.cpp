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

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboardMovement(LEFT, 0.1);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboardMovement(RIGHT, 0.1);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboardMovement(FORWARD, 0.1);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboardMovement(BACKWARD, 0.1);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.processKeyboardMovement(UP, 0.1);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.processKeyboardMovement(DOWN, 0.1);
	
	HandleMouse(window, camera); 


	
}

void Process::initMousePosition(GLFWwindow* window, Camera &camera, bool cursor_disabled){
	if (cursor_disabled){
		int height, width;
		glfwGetWindowSize(window, &width, &height);
		camera.lastX = width/2 - camera.Yaw*10;
		camera.lastY = height/2 + camera.Pitch*10;
		printf("size width %d, height %d\n", width, height);
		
	}else{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos); 
		camera.lastX = xpos - camera.Yaw*10;
		camera.lastY = ypos + camera.Pitch*10;
	}
}


void Process::HandleMouse(GLFWwindow* window, Camera &camera){

	double xpos, ypos;
	float xoffset, yoffset;
	
	glfwGetCursorPos(window, &xpos, &ypos); 
	
	xoffset = xpos - camera.lastX;
	yoffset = ypos - camera.lastY; 

	if (xoffset || yoffset)
		camera.processMouseMovement(xoffset, yoffset, 1);

	camera.CreateCallBacks(window, xoffset, yoffset); 

}


Process::~Process(){}