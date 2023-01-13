#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>

#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include "display.h"
#include "utils.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT, 
    UP,
    DOWN
};

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
     // Default camera values
    float YAW; // = -90.0f;
    float PITCH; //= 0.0f;
    float SPEED = 2.0f;
    float SENSITIVITY = 0.05f;
    float ZOOM = 60.0f;
   
    // camera Attributes
    
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    // euler Angles

    // mouse position when start program
    float initRunX;
    float initRunY;

    // constructor with vectors
    
    // constructor with scalar values
    Camera(glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f), float Yaw = -90.0f, float Pitch =  0.0f); // : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 getViewMatrix();

    glm::mat4 getProjectionMatrix(GLFWwindow* window, float near, float far);

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void processKeyboardMovement(Camera_Movement direction, float deltaTime);


    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    /* The motivated students can implement rotation using the mouse rather than the keyboard
        * You can draw inspiration from the ProcessKeyboardMovement function
    */

//    vois ScrollHandler(GLFWwindow * window); 

    void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch);
    
    void deactivateMouse(Display* display);
    void reactivateMouse(Display* display);

        
    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void static ScrollCallBack(GLFWwindow* window, double xoffset, double yoffset);

    void CreateCallBacks(GLFWwindow* window, double xoffset, double yoffset); 

    // getters
    glm::vec3 getPosition(){return position;}
    glm::vec3 getDirection(){return normalize(front);}
    float getYaw(){return yaw;}
    float getPitch(){return pitch;}
    float getMouseSensitivity(){return mouseSensitivity;}
    float getZoom(){return zoom;}



private:
    float yaw;
    float pitch;
    float zoom;
    // camera options
    float movementSpeed;
    float mouseSensitivity;

    glm::vec3 position;
    glm::vec3 front;
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
};