#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch){
    this->Front = glm::vec3(0.0f, 0.0f, -1.0f); 
    this->MovementSpeed = SPEED; 
    this->MouseSensitivity = SENSITIVITY; 
    this->Zoom = ZOOM; 
    this->Position = position; 
    this->WorldUp = up;
    this->Yaw = yaw;
    this->Pitch = pitch;
    this->updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix(){
    return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}

glm::mat4 Camera::getProjectionMatrix(GLFWwindow* window, float near, float far){
	int width, height;
	glfwGetWindowSize(window, &width, &height);
    return glm::perspective(glm::radians(this->Zoom), (float) width/height, near, far);
}

void Camera::processKeyboardMovement(Camera_Movement direction, float deltaTime){
    float velocity = this->MovementSpeed * deltaTime;
    if (direction == FORWARD)
        this->Position += normalize(this->Front * glm::vec3(1.0, 0.0, 1.0)) * velocity;
    if (direction == BACKWARD)
        this->Position -= normalize(this->Front * glm::vec3(1.0, 0.0, 1.0)) * velocity;
    if (direction == LEFT)
        this->Position -= normalize(this->Right) * velocity;
    if (direction == RIGHT)
        this->Position += normalize(this->Right) * velocity;
    if (direction == UP)
        this->Position += normalize(this->Up * glm::vec3(0.0, 1.0, 0.0)) * velocity;
    if (direction == DOWN)
        this->Position -= normalize(this->Up * glm::vec3(0.0, 1.0, 0.0)) * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch=true){
    float YawRot = 0;
    float PitchRot = 0;

    if (xoffset)
        YawRot += xoffset*MouseSensitivity;
    if (yoffset)
        PitchRot -= yoffset*MouseSensitivity;

    this->Yaw = YawRot;
    this->Pitch = PitchRot;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (this->Pitch > 89.0f)
            this->Pitch = 89.0f;
        if (this->Pitch < -89.0f)
            this->Pitch = -89.0f;
    }   
    updateCameraVectors();
}

// void Camera::ProcessMouseScroll(float yoffset){
//     ZOOM -= (float)yoffset;
//     if (ZOOM < 1.0f)
//         ZOOM = 1.0f;
//     if (ZOOM > 45.0f)
//         ZOOM = 45.0f;
// }

// void Camera::ScrollHandler(GLFWwindow * window){
//     glfwGetWindowUserPointer(window, reinterpret_cast<Camera *>(this));

// }

void Camera::ScrollCallBack(GLFWwindow* window, double xoffset, double yoffset){


    Camera *camera = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window)); 

    if (camera)
        camera->ZOOM -= (float)yoffset;
        if (camera->ZOOM < 1.0f)
            camera->ZOOM = 1.0f;
        if (camera->ZOOM > 45.0f)
            camera->ZOOM = 45.0f;
}

void Camera::CreateCallBacks(GLFWwindow* window, double xoffset, double yoffset){
    glfwSetScrollCallback(window, ScrollCallBack); 
}

void Camera::updateCameraVectors(){
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
}