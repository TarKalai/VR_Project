#include "process.h"

Process::Process(Display* displayArg, Camera* cameraArg, PhysicalWorld* worldArg, Shader* shaderArg, Shader* shadowArg) {
	display = displayArg;
	window = display->getWindow();
	camera = cameraArg;
	world = worldArg;
	shader = shaderArg;
	shadow = shadowArg;
}


void Process::processInput() {
	HandleMenuMode();
	HandleWindow();
	AnimationSpeed();
	PlacingParameter();
	PlacingDomino();
	Pushing();
	Deplacement();
	if (!Time::pause()) {
		HandleMouse();
	}
}

void Process::HandleMenuMode() {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		menuPressed  = true;
	}
	else if (menuPressed) {
		if (Time::pause()) {  // CONTINUE
			display->cursor_disabled = true;
			camera->reactivateMouse(display);
			glfwSetCursorPos(window, oldCursorX, oldCursorY);
			Time::setSpeed(sliderSpeedAnimation);
			Time::setPause(false);
		} else { // PAUSE
			Time::setPause(true);
			sliderSpeedAnimation = Time::getSpeed();
			if (menuPressed) { Time::setSpeed(0.);}
			glfwGetCursorPos(window, &oldCursorX, &oldCursorY);
    		display->cursor_disabled = false;
			camera->deactivateMouse(display);
		}
		menuPressed = false;
	}
}

void Process::HandleWindow() {
	QuitScreen();
	DecreaseScreen();
	IncreaseScreen();
	// FullScreen();
}

void Process::QuitScreen() {
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


void Process::DecreaseScreen() {
	if (glfwGetKey(window, GLFW_KEY_F10) == GLFW_PRESS && !fullscreen) {
		if (!decreaseResolution) {
			screenSize = std::max(3, (screenSize-1));
			int xpos, ypos;
			glfwGetWindowPos(window, &xpos, &ypos);
			glfwSetWindowMonitor(window, 0,  xpos, ypos, screenSize*1920/10, screenSize*1080/10, GLFW_DONT_CARE);
		}
		decreaseResolution = true;
	} else { decreaseResolution = false; }
}

void Process::IncreaseScreen() {
	if (glfwGetKey(window, GLFW_KEY_F12) == GLFW_PRESS && !fullscreen) {
		if (!increaseResolution) {
			screenSize = std::min(screenSize+1, 20);
			int xpos, ypos;
			glfwGetWindowPos(window, &xpos, &ypos);
			glfwSetWindowMonitor(window, 0, xpos, ypos, screenSize*1920/10, screenSize*1080/10, GLFW_DONT_CARE);
		}
		increaseResolution = true;
	} else { increaseResolution = false; }
}

void Process::FullScreen() {
	if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS) 
		resizescreen = true;
	else if (resizescreen) {
		resizescreen = false;
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		if (fullscreen) {
			glfwSetWindowMonitor(window, 0, oldWindowX, oldWindowY, width, height, GLFW_DONT_CARE);
			glfwSetCursorPos(window, oldCursorX, oldCursorY);
		}
		else {
			glfwGetWindowPos(window, &oldWindowX, &oldWindowY);
			glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, width, height, GLFW_DONT_CARE);
			initMousePosition();
			glfwGetCursorPos(window, &oldCursorX, &oldCursorY);
		}
		fullscreen = !fullscreen;
	}
}

void Process::AnimationSpeed() {
	// Animation speed
	if (glfwGetKey(window, GLFW_KEY_KP_0) == GLFW_PRESS) {
		sliderSpeedAnimation = 0;
		Time::setSpeed(0.);
	} else if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS) {
		sliderSpeedAnimation = 0.25;
		Time::setSpeed(0.25);
	} else if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS) {
		sliderSpeedAnimation = 1.;
		Time::setSpeed(1.);
	} else if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS) {
		sliderSpeedAnimation = 4.;
		Time::setSpeed(4.);
	}
}

void Process::PlacingParameter() {
	// SIZE
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		scaleIncrease = false;
		scaleDecrease = true;
	} 
	else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		scaleIncrease = true;
		scaleDecrease = false;
	}
	else {
		scaleIncrease = false;
		scaleDecrease = false;
	}
	// COLOR
	// RED
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		colorDomino.x -= 0.01;
	}
	else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		colorDomino.x += 0.01;
	}
	// GREEN
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		colorDomino.y -= 0.01;
	}
	else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		colorDomino.y += 0.01;
	}
	// BLUE
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		colorDomino.z -= 0.01;
	}
	else if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
		colorDomino.z += 0.01;
	}
	// RANDOM
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
		colorDomino = Utils::getRandom3();
	}
	// RAINBOW
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
		colorDomino = Utils::RainbowColor(glfwGetTime());
	}
	colorDomino = normalize(colorDomino);

}

void Process::PlacingDomino() {
	// Putting Domino
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) 
		PutDominoes();
	else
		firstDomino = true;
}

void Process::Pushing() { 
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)  {
		shoot = true;
		pressed += 1;
	} else if (shoot) {
		shoot = false;
		Object* sphere = new Object(geometry::sphere, Textures::White(), Materials::Empty(), camera->getPosition(), glm::vec3(0.), glm::vec3(1.), false); // visible=false
		world->addSphere(sphere, camera->getDirection()*glm::vec3(pressed), 30); // lifetime = 30
		pressed = 0;
	}
}

void Process::Deplacement() {
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera->processKeyboardMovement(LEFT, 0.1);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera->processKeyboardMovement(RIGHT, 0.1);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera->processKeyboardMovement(FORWARD, 0.1);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera->processKeyboardMovement(BACKWARD, 0.1);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		camera->processKeyboardMovement(UP, 0.1);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
		camera->processKeyboardMovement(DOWN, 0.1);
}

void Process::initMousePosition(){
	if (display->getCursorDisabled()){
		int height, width;
		glfwGetWindowSize(window, &width, &height);
		camera->initRunX = width/2 - camera->getYaw()*(1/camera->getMouseSensitivity());
		camera->initRunY = height/2 + camera->getPitch()*(1/camera->getMouseSensitivity());
		printf("size width %d, height %d\n", width, height);
	}
	else {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos); 
		camera->initRunX = xpos - camera->getYaw()*(1/camera->getMouseSensitivity());
		camera->initRunY = ypos + camera->getPitch()*(1/camera->getMouseSensitivity());
	}
}


void Process::HandleMouse(){

	double xpos, ypos;
	float xoffset, yoffset;
	
	glfwGetCursorPos(window, &xpos, &ypos); 
	
	xoffset = xpos - camera->initRunX;
	yoffset = ypos - camera->initRunY; 

	if (xoffset || yoffset)
		camera->processMouseMovement(xoffset, yoffset, 1);

	camera->CreateCallBacks(window, xoffset, yoffset); 

}

void Process::PutDominoes(){
	float espacement = scaleDomino*(dominoDim::height/2 + dominoDim::thick); // distance between 2 domino

	glm::vec3 dir = camera->getDirection(); 
	glm::vec3 pos = camera->getPosition();

	if (dir.y < 0) {
		double ratio = (dominoDim::height/2 - pos.y)/dir.y;
		glm::vec3 cursorPosition = glm::vec3(pos.x+ratio*dir.x, pos.y+ratio*dir.y, pos.z+ratio*dir.z);
		
		if (firstDomino) {
			firstDomino = false;
			lastDomino = cursorPosition;
		} 
		else  {
			float dist = glm::distance(lastDomino, cursorPosition);
			if (dist > espacement) { 
				if (scaleIncrease) { scaleDomino = glm::min(10., scaleDomino*1.1); }
				else if (scaleDecrease) { scaleDomino = glm::max(0.2, scaleDomino*0.9); }
				ratio = espacement/dist;
				glm::vec3 nextDomino = glm::vec3(1-ratio)*lastDomino + glm::vec3(ratio)*cursorPosition; // To get dominoes at constant interval
				glm::vec3 delta_dir = nextDomino-lastDomino;
				Object* domino = new Object(geometry::domino, Textures::White(), Materials::Shiny(), 
											glm::vec3(lastDomino.x, scaleDomino, lastDomino.z), glm::vec3(0., -glm::atan(delta_dir.z/delta_dir.x), 0.), glm::vec3(scaleDomino), 
											true, normalize(colorDomino));	
				world->addDomino(domino);
				shader->addObject(domino);
				shadow->addObject(domino);
				lastDomino = nextDomino; // go to next domino
			}
		}
	}
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
	glm::mat4 view = camera->getViewMatrix();
	glm::mat4 projection = camera->getProjectionMatrix(window, near, far);

	glm::vec4 farScreen = glm::vec4(xpos*far, ypos*far, far-0.015, far);
	glm::vec4 farModel = glm::inverse(projection*view)*farScreen;
	glm::vec4 farCoord = glm::vec4(farModel/farModel.w);
	std::cout << glm::to_string(farCoord) << std::endl;

	// TODO with that
	// std::cout << "cam " << glm::to_string(camera->Front) << std::endl;

	
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

		world->addCube(cube);
		shader->addObject(cube);
	}
}*/

Process::~Process(){}