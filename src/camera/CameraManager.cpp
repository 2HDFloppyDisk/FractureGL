#include "camera/CameraManager.h"

// Constructor
CameraManager::CameraManager() {}

// Destructor
CameraManager::~CameraManager() {}

// Initialize the camera with a position
void CameraManager::InitCamera(const glm::vec3& position) {
    camera = Camera(position);  // Directly initialize the camera object
}

// Get the reference to the camera
Camera& CameraManager::GetCamera() {
    return camera;  // Return a reference to the camera object
}
