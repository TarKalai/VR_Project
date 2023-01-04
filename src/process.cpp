#include "process.h"
#include "camera.h"
#include "string.h"
#include "display.h"

#include <glm/glm.hpp>
#include "glm/gtx/string_cast.hpp" // (print matrix) debug purpose

Process::Process() {}


void Process::processInput(GLFWwindow* window, Camera &camera, PhysicalWorld &world, Shader &shader) {


	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS) 
		resizescreen = true;
	else if (resizescreen) {
		resizescreen = false;
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		if (fullscreen) 
			glfwSetWindowMonitor(window, 0, 0, 0, width, height, GLFW_DONT_CARE);
		else 
			glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, width, height, GLFW_DONT_CARE);
		fullscreen = !fullscreen;
	}
	if (glfwGetKey(window, GLFW_KEY_F10) == GLFW_PRESS && !fullscreen) {
		if (!decreaseResolution) {
			screenSize = std::max(3, (screenSize-1));
			glfwSetWindowMonitor(window, 0,  0, 0, screenSize*1920/10, screenSize*1080/10, GLFW_DONT_CARE);
		}
		decreaseResolution = true;
	} else { decreaseResolution = false; }
	if (glfwGetKey(window, GLFW_KEY_F12) == GLFW_PRESS && !fullscreen) {
		if (!increaseResolution) {
			screenSize = std::min(screenSize+1, 20);
			glfwSetWindowMonitor(window, 0, 0, 0, screenSize*1920/10, screenSize*1080/10, GLFW_DONT_CARE);
		}
		increaseResolution = true;
	} else { increaseResolution = false; }

	if (glfwGetKey(window, GLFW_KEY_KP_ENTER) == GLFW_PRESS) {
		pausePressed = true;
		pause = !pause;
	} else if (pausePressed) { 
		pausePressed = false;
		if (pause)
			world.speedAnimation = 0.; 
		else
			world.speedAnimation = 1.;
	}
	
	if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) 
		world.speedAnimation = 0.5;
	else if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) 
		world.speedAnimation = 10.;
	else if (!pause)
		world.speedAnimation = 1.;


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
		SaveCursorPath(window, camera, world, shader);
	else if (cursor_path.size() > 0) {
		PutDominoes(window, camera, world, shader);
	}
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)  {
		shoot = true;
		pressed += 1;
		std::cout << "-";
	} else if (shoot) {
		shoot = false;
		world.push(camera.Position, camera.Front, pressed);
		pressed = 0;
	}
	HandleMouse(window, camera);
}

void Process::initMousePosition(GLFWwindow* window, Camera &camera, bool cursor_disabled){
	if (cursor_disabled){
		int height, width;
		glfwGetWindowSize(window, &width, &height);
		camera.initRunX = width/2 - camera.Yaw*(1/camera.MouseSensitivity);
		camera.initRunY = height/2 + camera.Pitch*(1/camera.MouseSensitivity);
		printf("size width %d, height %d\n", width, height);
	}
	else {
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

void Process::SaveCursorPath(GLFWwindow* window, Camera &camera, PhysicalWorld &world, Shader &shader){
	
	double size = 1;
	glm::vec3 dir = camera.Front;
	glm::vec3 pos = camera.Position;

	if (dir.y < 0) {
		double ratio = (size - pos.y)/dir.y;
		glm::vec3 destination = glm::vec3(pos.x+ratio*dir.x, pos.y+ratio*dir.y, pos.z+ratio*dir.z);
		cursor_path.push_back(destination);
	}
}

void Process::PutDominoes(GLFWwindow* window, Camera &camera, PhysicalWorld &world, Shader &shader){
	float heightDomino = 1.*2;
	float widthDomino = 0.175*2;
	float espacement = 0.5*heightDomino + widthDomino; // distance between 2 domino

	char cubeGeometry[] = "../../objects/domino.obj";
	float dist;
	float ratio;
	glm::vec3 next;
	glm::vec3 dir;
	glm::vec3 last = cursor_path[0];

	int i = 1;
    while (i < cursor_path.size()-1) {
		dist = glm::distance(last, cursor_path[i]);
		if (dist < espacement) { // if next domino too close, check the next one
        	i++;
		}
		else if (dist > espacement) { 
			ratio = espacement/dist;
			next = glm::vec3(1-ratio)*last + glm::vec3(ratio)*cursor_path[i]; // To get dominoes at constant interval
			dir = next-last;

			Object* cube = new Object(cubeGeometry, glm::vec3(last), glm::vec3(0., -glm::atan(dir.z/dir.x), 0.), glm::vec3(1.));	
			world.addDomino(cube);
			shader.addObject(cube);

			last = next;
		}
    }
	cursor_path.clear();
}


/* Putting domino at cursor position (keep if we need to do something similar in the futur)
void Process::SaveCursorPath(GLFWwindow* window, Camera &camera, PhysicalWorld &world, Shader &shader){

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
		Object* cube = new Object(cubeGeometry, glm::vec3(x, y, z), glm::vec3(0., 0, 0), glm::vec3(1.));	

		world.addCube(cube);
		shader.addObject(cube);
	}
}*/

Process::~Process(){}