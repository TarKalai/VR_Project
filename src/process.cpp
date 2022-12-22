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

void Process::HandleMouse(GLFWwindow* window, Camera &camera){

	double xpos, ypos, xpos_i, ypos_i;
	glfwGetCursorPos(window, &xpos, &ypos); 
	if (camera.firstmouse){
		camera.lastX = xpos + camera.Yaw*10;
		camera.lastY = -ypos + camera.Pitch*10;
		camera.firstmouse = false;
	}

	float xoffset = xpos - camera.lastX;
	float yoffset = -ypos - camera.lastY; 
	if (xoffset || yoffset)
		camera.processMouseMovement(xoffset, yoffset, 1);
	glfwGetCursorPos(window, &xpos_i, &ypos_i); 
	if (xpos_i - xpos != 0 | ypos_i - ypos !=0 )
		camera.firstmouse = true;
	
	// processMouseScroll(yoffset, camera); 
	// camera.processMouseScroll(yoffset); 
	camera.CreateCallBacks(window, xoffset, yoffset); 

	
	// if (glfwGetMouseButton(window, 1) == GLFW_PRESS){
	// 	// printf("I GOT PRESSED %f, %f", camera.ZOOM, yoffset); 
	// 	if (camera.ZOOM >= 1.0f && camera.ZOOM <= 45.0f)
    //     	camera.ZOOM -= yoffset;
	// 	if (camera.ZOOM < 1.0f)
	// 		camera.ZOOM = 1.0f;
	// 	if (camera.ZOOM > 45.0f)
	// 		camera.ZOOM = 45.0f;
	// }
	// camera.processMouseScroll(yoffset); 
}

// void Process::processMouseScroll(float yoffset, Camera &camera){
// 	// if (camera.ZOOM < 45.0f && camera.ZOOM > 1.0f)
//     camera.ZOOM -= (float)yoffset*0.00001;
//     if (camera.ZOOM <= 1.0f)
//         camera.ZOOM = 1.0f;
//     if (camera.ZOOM >= 45.0f)
//         camera.ZOOM = 45.0f;
// }

Process::~Process(){}