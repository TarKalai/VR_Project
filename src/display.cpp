#include "display.h"
#include "debug.h"
#include "process.h"
#include "camera.h"

float fov = 45; 

Display::Display(bool cursor){
    cursor_disabled = cursor;
}

int Display::Initialise(){
    // Initialise Glfw
    int width = 1920/2;
    int height = 1080/2;
    glfwTerminate();
    if(!glfwInit()){
        printf("The inititialisation of glfw failed.");
        glfwTerminate(); 
        return 1; 
    }
    

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want it to be backwards compatible.
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    #ifndef NDEBUG
	//create a debug context to help with Debugging
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    #endif

	//mainWindow = glfwCreateWindow(width, height, "Project", glfwGetPrimaryMonitor(), NULL); // Fullscreen
	mainWindow = glfwCreateWindow(width, height, "Project", NULL, NULL);
	glfwSetWindowMonitor(mainWindow, 0,  0, 0, width, height, GLFW_DONT_CARE);


    if(!mainWindow){
        printf("The window was not created.");
        glfwTerminate(); 
        return 1; 
    }

    glfwMakeContextCurrent(mainWindow);  // Everything that will be done will bejoined to this window

    glfwSetFramebufferSizeCallback(mainWindow, framebuffer_size_callback);
    // glfwSetScrollCallback(mainWindow, scroll_callback);
    
    if (cursor_disabled)
	    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);// GLFW_CURSOR_DISABLED so that the cursor does not appear on the screen. 


    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		printf("Failed to initialize GLAD");
	}

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE); // If the objects are behind the camera then they will not be rendered => Makes everything go faster. 
    // This may cause a bug with the Normals, if a bug is observed (holes in objects) disabling this line may solve it. 

    glViewport(0, 0, bufferWidth, bufferHeight);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
    // Setup Dear ImGui style
    ImGui::StyleColorsLight();
    //ImGui::StyleColorsLight();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(mainWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");

#ifndef NDEBUG
	int flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT){
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
#endif
    return 0;  
}

void Display::framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // make sure the viewport matches the new window dimensions;
    glViewport(0, 0, width, height);
}

void Display::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    if (fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;
}

Point Display::getCenter() {
    int width, height;
	int xpos, ypos;
	glfwGetWindowPos(mainWindow, &xpos, &ypos);
	glfwGetWindowSize(mainWindow, &width, &height);

    return Point{xpos+width/2, ypos+height/2};
}

int Display::getWidth() {
    int width, height;
	glfwGetWindowSize(mainWindow, &width, &height);
    return width;
}

int Display::getHeight() {
    int width, height;
	glfwGetWindowSize(mainWindow, &width, &height);
    return height;
}

void Display::updateGUI() {
    displayFPS();
    //displaySaveLoad();
    displaySpeedAnimation();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

float* Display::getFPS() {
	double now = glfwGetTime();
    double deltaTime = now - prev;
    deltaFrame++;
    if (deltaTime > 0.5) {
        prev = now;
        fps = (float)deltaFrame / deltaTime;
        deltaFrame = 0;
    }
	return &fps;
}

void Display::displayFPS() {
    Point size = Point({80, 20});
    ImGui::SetNextWindowPos(ImVec2(getWidth()-size.x, getHeight()-size.y));
    ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
    ImGui::Begin("fps", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings);
    ImGui::SetCursorPos(ImVec2(0,0));
    ImGui::InputFloat("fps", getFPS(), 0, 0, "%.1f", ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_CharsNoBlank);
    ImGui::End();
}

void Display::displaySaveLoad() {
    if (cursor_disabled) {
        ImGui::Begin("My name is window", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
		ImGui::Text("Hello world");
		bool check;
		ImGui::Checkbox("CHECK", &check);
		std::cout << "check:" << check << std::endl;
		float slider;
		ImGui::SliderFloat("SLIDER", &slider, 0., 1.);
		std::cout << "slider:" << slider << std::endl;
		//float testcolor[3] = {1.,1.,0.};
		//ImGui::ColorEdit3("COLOR", testcolor);
		ImGui::End();
    }
    else {
    }
}

void Display::displaySpeedAnimation() {
    Point size = Point({80, 20});
    ImGui::SetNextWindowPos(ImVec2(getWidth()-size.x, 0));
    ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
    ImGui::Begin("Animation Speed", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
    //ImGui::SliderFloat("spped", &world->speedAnimation, 0., 1.);
    ImGui::End();
}

Display::~Display(){
    glfwDestroyWindow(mainWindow);
    glfwTerminate(); 
}

