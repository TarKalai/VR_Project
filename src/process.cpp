#include "process.h"
#include "camera.h"
#include "string.h"



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

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
		double xpos, ypos;
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
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
		camera.firstmouse = true;
}

Process::~Process(){}