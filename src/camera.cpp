#include "camera.h"


glm::mat4 Camera::getViewMatrix(){
    return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}

glm::mat4 Camera::getProjectionMatrix(float fov, float ratio, float near, float far){
    return glm::perspective(fov, ratio, near, far);
}

void Camera::processKeyboardMovement(Camera_Movement direction, float deltaTime){
    float velocity = this->MovementSpeed * deltaTime;
    if (direction == FORWARD)
        this->Position += this->Front * velocity;
    if (direction == BACKWARD)
        this->Position -= this->Front * velocity;
    if (direction == LEFT)
        this->Position -= this->Right * velocity;
    if (direction == RIGHT)
        this->Position += this->Right * velocity;
    if (direction == UP)
        this->Position += this->Up * velocity;
    if (direction == DOWN)
        this->Position -= this->Up * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch=true){
    float YawRot = 0;
    float PitchRot = 0;

    if (xoffset)
        YawRot -= xoffset*.1;
    if (yoffset)
        PitchRot -= yoffset*.1;

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

void Camera::processMouseScroll(float yoffset){
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
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