#include "display.h"
#include "debug.h"
#include "process.h"
#include "camera.h"
#include "utils.h"

float fov = 45;

Display::Display(bool cursor){
    cursor_disabled = cursor;
    width = 1920/2;
    height = 1080/2;
}


int Display::Initialise()
{
    // Initialise Glfw
    if(!glfwInit()){
        printf("The inititialisation of glfw failed.");
        glfwTerminate(); 
        return 1; 
    }
    
    // Set the window properties

    // Set the version of glfw to use to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_SAMPLES, general::samples);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want it to be backwards compatible.
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // We want it to be forward compatible

    #ifndef NDEBUG
	//create a debug context to help with Debugging
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    #endif

    mainWindow = glfwCreateWindow(width, height, "Project", NULL, NULL); 


    // glfwSetWindowMonitor(mainWindow, 0,  0, 0, width, height, GLFW_DONT_CARE);
    // 1st NULL: which monitor want to use  
    // 2scd NULL If we want to share the window across systems.  


    if(!mainWindow){
        printf("The window was not created.");
        glfwTerminate(); 
        return 1; 
    }

    glfwMakeContextCurrent(mainWindow);  // Everything that will be done will bejoined to this window

    glfwSetFramebufferSizeCallback(mainWindow, framebuffer_size_callback);

    if (cursor_disabled)
	    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);// GLFW_CURSOR_DISABLED so that the cursor does not appear on the screen. 

    
    // Buffer size: the buffer is  the part which is going to hold all the Opengl data
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight); 

    //Handle Key and Mouse INput
    // createCallbacks(); 

    // glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);// so that the cursor does not appear on the screen. 


    //Set the size of the part we are drawing to on the window
    // We want it to be the entire window

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD");
	}

    glEnable(GL_DEPTH_TEST); //Enable depth testing to determine which trangle is deeper into the image. 

    glEnable(GL_MULTISAMPLE); 

    glViewport(0, 0, bufferWidth, bufferHeight);

    // glfwSetWindowUserPointer(mainWindow, this); // this: owner of the window => refers to the class. 

     // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsLight();
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

void Display::resetViewport() {
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
    glViewport(0, 0, bufferWidth, bufferHeight);
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



// void Display::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
// {
//     Display* theWindow = static_cast<Display*>(glfwGetWindowUserPointer(window)); 

//     if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
//         glfwSetWindowShouldClose(window, GL_TRUE);
//     }

//     if(key >= 0 && key < 1024){
//         if(action == GLFW_PRESS){
//             theWindow->keys[key]=true; 
//         }
//         else if (action == GLFW_RELEASE){
//             theWindow->keys[key]=false; 
//         }
//     }

// }

// void Display::handleMouse(GLFWwindow* window, double xPos, double yPos)
// {
//     Display* theWindow = static_cast<Display*>(glfwGetWindowUserPointer(window)); 

//     if(theWindow->mouseFirstMoved)
//     {
//         theWindow->lastX=xPos; 
//         theWindow->lastY=yPos;
//         theWindow->mouseFirstMoved=false;  
//     }

//     theWindow->xChange = xPos - theWindow->lastX; 
//     theWindow->yChange = theWindow->lastY - yPos; 

//     theWindow->lastX = xPos; 
//     theWindow->lastY = yPos; 

// }

// void Display::createCallbacks(){
//     glfwSetKeyCallback(mainWindow, handleKeys); // When a key i spressed on mainWindow then callback to handleKeys. 
//     glfwSetCursorPosCallback(mainWindow, handleMouse);
// }

// GLfloat Display::getXchange()
// {
//     GLfloat theChange = xChange; 
//     xChange = 0.0;
//     return theChange;
// }

// GLfloat Display::getYchange()
// {
//     GLfloat theChange = yChange; 
//     yChange = 0.0; 
//     return theChange; 
// }

Display::~Display()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate(); 

}