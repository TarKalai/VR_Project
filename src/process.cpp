#include "process.h"

Process::Process(Display* displayArg, Camera* cameraArg, PhysicalWorld* worldArg, Shader* shaderArg, Shader* shadowArg, Shader* omniShadowShader) {
	display = displayArg;
	window = display->getWindow();
	camera = cameraArg;
	world = worldArg;
	shader = shaderArg;
	shadow = shadowArg;
	omniShadow = omniShadowShader;
}


void Process::processInput() {
	HandleMenuMode();
	HandleWindow();
	AnimationSpeed();
	if (!Time::pause()) {
		PlacingParameter();
		DeleteDominos();
		PlacingDomino();
		ModifyDomino();
		Pushing();
		Deplacement();
		HandleMouse();
		FlashLight();
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
	if (glfwGetKey(window, GLFW_KEY_V) != GLFW_PRESS) { // If not "Torch mode"
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
}

void Process::PlacingDomino() {
	// Putting Domino
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) 
		PutDominos();
	else
		firstDomino = true;
}

void Process::Pushing() { 
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)  {
		shoot = true;
		enterPressed += 1;
	} else if (shoot) {
		shoot = false;

		glm::vec3 dir = camera->getDirection(); 
		glm::vec3 pos = camera->getPosition();

		double dist = 200; // Distance to push object (May vary depending of the farplane)
		glm::vec3 to = glm::vec3(pos.x+dist*dir.x, pos.y+dist*dir.y, pos.z+dist*dir.z);

		world->RayCastPush(camera->getPosition(), to, PHYSIC::NORMAL_OBJECT, enterPressed);
		enterPressed = 0;
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

void Process::ModifyDomino() {
	if (glfwGetKey(window, GLFW_KEY_SEMICOLON) == GLFW_PRESS) {
		glm::vec3 dir = camera->getDirection(); 
		glm::vec3 pos = camera->getPosition();

		double dist = 200; // Distance to delete object (May vary depending of the farplane)
		glm::vec3 to = glm::vec3(pos.x+dist*dir.x, pos.y+dist*dir.y, pos.z+dist*dir.z);

		selectedDomino = world->RayCastObj(camera->getPosition(), to, PHYSIC::NORMAL_OBJECT); // See GUI for next step
	} else { selectedDomino = nullptr; }
}

void Process::DeleteDominos() {
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		glm::vec3 dir = camera->getDirection(); 
		glm::vec3 pos = camera->getPosition();

		double dist = 200; // Distance to delete object (May vary depending of the farplane)
		glm::vec3 to = glm::vec3(pos.x+dist*dir.x, pos.y+dist*dir.y, pos.z+dist*dir.z);
		int objID = world->DeleteRayCastObj(camera->getPosition(), to, PHYSIC::NORMAL_OBJECT);
		shader->remove(objID);
		shadow->remove(objID);
	}
}

void Process::PutDominos(){
	float espacement = scaleDomino*(dimDomino.y/2 + dimDomino.z);//dominoDim::height/2 + dominoDim::thick); // distance between 2 domino

	glm::vec3 dir = camera->getDirection(); 
	glm::vec3 pos = camera->getPosition();

	if (dir.y < 0) {
		double ratio = -pos.y/dir.y;
		glm::vec3 to = glm::vec3(pos.x+ratio*dir.x, pos.y+ratio*dir.y, pos.z+ratio*dir.z);
		glm::vec3 destination = world->RayCastPos(camera->getPosition(), to, PHYSIC::GROUND_OBJECT);

		if (destination.y != -1) {
			if (firstDomino) {
				firstDomino = false;
				lastDomino = destination;
			} 
			else  {
				float dist = glm::distance(lastDomino, destination);
				if (dist > espacement) { 
					if (scaleIncrease) { scaleDomino = glm::min(10., scaleDomino*1.1); }
					else if (scaleDecrease) { scaleDomino = glm::max(0.3, scaleDomino*0.9); }
					
					ratio = espacement/dist;
					glm::vec3 nextDomino;
					nextDomino.x = (1-ratio)*lastDomino.x + ratio*destination.x;
					nextDomino.z = (1-ratio)*lastDomino.z + ratio*destination.z;
					nextDomino.y = destination.y;
					glm::vec3 delta_dir = nextDomino-lastDomino;
					if (abs(nextDomino.y - lastDomino.y) < 0.1){
						Object* domino = new Object(geometryDomino, textureDomino, materialDomino, 
													glm::vec3(lastDomino.x, lastDomino.y+scaleDomino, lastDomino.z), glm::vec3(0., -glm::atan(delta_dir.z/delta_dir.x), 0.), glm::vec3(scaleDomino), 
													normalize(colorDomino));			
						world->addObject(domino);
						shader->addObject(domino);
						shadow->addObject(domino);		
						omniShadow->addObject(domino);		
					}
					else 
						firstDomino = true;
					lastDomino = nextDomino; // go to next domino
				}
			}
		}
	}
}

void Process::FlashLight() {
	glm::vec3 color = Torch::getColor();

	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) { // Torch mode
		Torch::setOn(true);
		// INTENSITY
		// ambiant
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
			Torch::setIntensity(glm::max(0., Torch::getIntensity()-0.05));
		} 
		else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
			Torch::setIntensity(glm::min(5., Torch::getIntensity()+0.05));
		}
		// SIZE
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
			Torch::setSize(glm::max(10.f, Torch::getSize()-1));
		} 
		else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
			Torch::setSize(glm::min(80.f, Torch::getSize()+1));
		}
		// COLOR
		// RED
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
			color.x -= 0.01;
		}
		else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
			color.x += 0.01;
		}
		// GREEN
		if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
			color.y -= 0.01;
		}
		else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
			color.y += 0.01;
		}
		// BLUE
		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
			color.z -= 0.01;
		}
		else if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
			color.z += 0.01;
		}
		// RANDOM
		if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
			color = Utils::getRandom3();
		}
		// RAINBOW
		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
			color = Utils::RainbowColor(glfwGetTime());
		}
		Torch::setColor(normalize(color));
	}
	else { Torch::setOn(false); }
}

Process::~Process(){}