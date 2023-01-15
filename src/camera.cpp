#include "camera.h"

Camera::Camera(glm::vec3 Position, glm::vec3 Up, float Yaw, float Pitch){
    this->front = glm::vec3(0.0f, 0.0f, -1.0f); 
    this->movementSpeed = SPEED; 
    this->mouseSensitivity = SENSITIVITY; 
    this->zoom = ZOOM; 
    this->position = Position; 
    this->worldUp = Up;
    this->yaw = Yaw;
    this->pitch = Pitch;
    this->updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix(){
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix(GLFWwindow* window, float near, float far){
    int width, height;
	glfwGetWindowSize(window, &width, &height);
    return glm::perspective(glm::radians(zoom), (float) width/height, near, far);
}

// glm::mat4 Camera::getViewProjectionMatrix()
// {
//     glm::
//     return 
// }

void Camera::processKeyboardMovement(Camera_Movement direction, float deltaTime){
    float velocity = movementSpeed * deltaTime;
    if (direction == FORWARD)
        position += normalize(front * glm::vec3(1.0, 0.0, 1.0)) * velocity;
    if (direction == BACKWARD)
        position -= normalize(front * glm::vec3(1.0, 0.0, 1.0)) * velocity;
    if (direction == LEFT)
        position -= normalize(right) * velocity;
    if (direction == RIGHT)
        position += normalize(right) * velocity;
    if (direction == UP)
        position += normalize(up * glm::vec3(0.0, 1.0, 0.0)) * velocity;
    if (direction == DOWN)
        position -= normalize(up * glm::vec3(0.0, 1.0, 0.0)) * velocity;
}

void Camera::deactivateMouse(Display* display) {
    glfwSetInputMode(display->getWindow(), GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
    mouseSensitivity = 0;
}

void Camera::reactivateMouse(Display* display) {
    glfwSetInputMode(display->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    mouseSensitivity = SENSITIVITY;
}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch=true){

    float YawRot = 0;
    float PitchRot = 0;

    if (xoffset)
        YawRot += xoffset*mouseSensitivity;
    if (yoffset)
        PitchRot -= yoffset*mouseSensitivity;

    if (!Time::pause()) {
        yaw = YawRot;
        pitch = PitchRot;
    }

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }   
    updateCameraVectors();
    
}

void Camera::ScrollCallBack(GLFWwindow* window, double xoffset, double yoffset){


    Camera *camera = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window)); 
    if (camera)
        camera->zoom -= (float) yoffset;
        if (camera->zoom < 1.0f)
            camera->zoom= 1.0f;
        if (camera->zoom > 45.0f)
            camera->zoom = 45.0f;
}

void Camera::CreateCallBacks(GLFWwindow* window, double xoffset, double yoffset){
    glfwSetScrollCallback(window, ScrollCallBack); 
}

void Camera::updateCameraVectors(){
    // calculate the new Front vector
   
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up = glm::normalize(glm::cross(right, front));
}