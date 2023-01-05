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

Display::~Display(){
    glfwDestroyWindow(mainWindow);
    glfwTerminate(); 
}

