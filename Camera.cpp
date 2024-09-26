#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include "LogUtils.h"

// Constructor with initial values
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Position(position), WorldUp(up), Yaw(yaw), Pitch(pitch),
    Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(2.5f), MouseSensitivity(0.1f)
{
    debugMessages.push_back("Camera system initialized!");  // Add test message once
    UpdateCameraVectors();
}

// Returns the view matrix using glm::lookAt
glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

// Returns the projection matrix
glm::mat4 Camera::GetProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane)
{
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}


// Process input for keyboard movement
void Camera::ProcessKeyboard(char direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == 'W')  // Forward
        Position += Front * velocity;
    if (direction == 'S')  // Backward
        Position -= Front * velocity;
    if (direction == 'A')  // Left
        Position -= Right * velocity;
    if (direction == 'D')  // Right
        Position += Right * velocity;
}

// Process mouse movement for looking around
void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // Constrain pitch to avoid camera flipping
    if (Pitch > 89.0f)
        Pitch = 89.0f;
    if (Pitch < -89.0f)
        Pitch = -89.0f;

    // Update Front, Right, and Up Vectors based on updated Euler angles
    UpdateCameraVectors();
}

// Update the camera's direction vectors based on the new yaw and pitch angles
void Camera::UpdateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors
    Up = glm::normalize(glm::cross(Right, Front));
}





// Sets a new position for the camera
void Camera::SetPosition(const glm::vec3& position)
{
    Position = position;
    UpdateCameraVectors();
}

// Sets a new direction for the camera
void Camera::SetDirection(const glm::vec3& front)
{
    Front = front;
    UpdateCameraVectors();
}

// Getters for position and direction
glm::vec3 Camera::GetPosition() const
{
    return Position;
}

glm::vec3 Camera::GetDirection() const
{
    return Front;
}


