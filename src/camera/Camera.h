#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // Euler Angles
    float Yaw;
    float Pitch;

    // Constructor with default camera values
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = -90.0f,
        float pitch = 0.0f);

    // Returns the view matrix
    glm::mat4 GetViewMatrix();

    // Returns the projection matrix
    glm::mat4 GetProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane);

    // Camera movement
    void SetPosition(const glm::vec3& position);
    void SetDirection(const glm::vec3& front);

    // Camera options
    float MovementSpeed;
    float MouseSensitivity;

    // Getters for position and direction
    glm::vec3 GetPosition() const;
    glm::vec3 GetDirection() const;

    void ProcessKeyboard(char direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset);

private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void UpdateCameraVectors();
};

#endif
