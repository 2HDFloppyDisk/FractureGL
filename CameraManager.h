#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include "Camera.h"

class CameraManager {
private:
    Camera camera;  // Camera as a direct class member (no need for pointers)

public:
    // Constructor
    CameraManager();

    // Destructor
    ~CameraManager();

    // Initialize the camera with a position
    void InitCamera(const glm::vec3& position);

    // Get the reference to the camera
    Camera& GetCamera();

    // No need for CleanupCamera as we are not using dynamic memory
};

#endif // CAMERAMANAGER_H
