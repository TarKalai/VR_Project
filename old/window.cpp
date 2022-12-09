#include <stdio.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

// #include<iostream>



// Window Dimensions
const GLint WIDTH = 1000, HEIGHT = 700; //GLint is the type of opengl we are going to use.

int main(){

    // Initialise Glfw
    if(!glfwInit()){
        printf("The inititialisation of glfw failed.");
        glfwTerminate(); 
        return 1; 
    }
    
    // Set the window properties

    // Set the version of glfw to use to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want it to be backwards compatible.
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // We want it to be forward compatible

    GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Scene", NULL, NULL); 
    // 1st NULL: which monitor want to use
    // 2scd NULL If we want to share the window across systems.  

    if(!mainWindow){
        printf("The window was not created.");
        glfwTerminate(); 
        return 1; 
    }

    glfwMakeContextCurrent(mainWindow);  // Everything that will be done will bejoined to this window

    // Buffer size: the buffer is  the part which is going to hold all the Opengl data
    int bufferWidth, bufferHeight; 
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight); 

    //Set the size of the part we are drawing to on the window
    // We want it to be the entire window

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD");
	}


    
    


    glViewport(0, 0, bufferWidth, bufferHeight);
    
    while(!glfwWindowShouldClose(mainWindow)){

        // Get and Handle user input
        glfwPollEvents(); 

        glClearColor(1.0f, 0.0f, 1.0f, 1.0f); // Clear all the frame so that you will be able to draw another frame (can chose the color of the clear)

        glClear(GL_COLOR_BUFFER_BIT); // A pixel does not only have color as data, it also has depth and other things. We are specifying here that we want to clear the color. 

        glfwSwapBuffers(mainWindow); // There are 2 scenes going on at once, we are drawing to the one that can't be seen, and we call swapBuffers to swap them around: so then the one we are drawing to is the one that can be seen and the one which could be seen originaly is the one we are drawing to. 
    }
    return 0; 
}

