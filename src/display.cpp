#include "display.h"
#include "debug.h"
#include "process.h"

Display::Display()
{
    width=1900; 
    height=850; 
}

Display::Display(GLint windowWidth, GLint windowHeight)
{
    width = windowWidth; 
    height = windowHeight; 

}

int Display::Initialise()
{
    // Initialise Glfw
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

	mainWindow = glfwCreateWindow(width, height, "Project", NULL, NULL);

    if(!mainWindow){
        printf("The window was not created.");
        glfwTerminate(); 
        return 1; 
    }

    glfwMakeContextCurrent(mainWindow);  // Everything that will be done will bejoined to this window

	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);// so that the cursor does not appear on the screen. 


    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD");
	}

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, bufferWidth, bufferHeight);

#ifndef NDEBUG
	int flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	 glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
#endif
    return 0;  
}

Display::~Display()
    {
    glfwDestroyWindow(mainWindow);
    glfwTerminate(); 
    }

