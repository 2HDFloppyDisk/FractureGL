#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../camera/CameraManager.h"  // Update to CameraManager
#include "../graphics/GLUtils.h"
#include "../utils/LogUtils.h"
#include "../entities/SimpleCube.h"
#include "../ui/BackgroundVFX.h"
#include "../graphics/Render.h"
#include "ShaderUtils.h"
#include "../entities/SimpleArrow.h"
#include "../models/ModelLoader.h"

unsigned int VAO = 0;  // Define the variables
unsigned int VBO = 0;
unsigned int EBO = 0;

unsigned int arrowVAO = 0;  // Define the variables
unsigned int arrowVBO = 0;
unsigned int arrowEBO = 0;

// Global shader programs
GLuint fogShaderProgram;
GLuint arrowShaderProgram;

// Function to initialize rendering
void InitRendering() {
    // Enable depth testing for proper 3D rendering
    glEnable(GL_DEPTH_TEST);
}

// Function to setup the scene (shaders, objects, particles)
void SetupScene(Camera& camera, int windowWidth, int windowHeight) {
    debugMessages.push_back("Setting up scene...");

    // Load shaders
    fogShaderProgram = LoadShader("shaders/fog_vertex.glsl", "shaders/fog_fragment.glsl");
    if (fogShaderProgram == 0) {
        debugMessages.push_back("Failed to load fog shader.");
        return;
    }

    arrowShaderProgram = LoadShader("shaders/arrow_vertex.glsl", "shaders/arrow_fragment.glsl");
    if (arrowShaderProgram == 0) {
        debugMessages.push_back("Failed to load arrow shader.");
        return;
    }

    // Initialize particle systems (dust and fog particles)
    InitDustParticles(windowWidth, windowHeight);
    InitFogParticles(windowWidth, windowHeight);

    debugMessages.push_back("Scene setup complete.");
}

// Function to render the scene
void RenderScene(Camera& camera, GLuint fogShaderProgram, GLuint arrowShaderProgram) {
    // Clear the color and depth buffers
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear both the color and depth buffer

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);  // Set background to gray

    GLint isProgramValid;
    glValidateProgram(fogShaderProgram);
    glGetProgramiv(fogShaderProgram, GL_VALIDATE_STATUS, &isProgramValid);

    if (!isProgramValid) {
        debugMessages.push_back("Shader program not valid");
    }
    
    // Add error checks
    CheckOpenGLError("OpenGL error after validating shader program: ");

    GLint viewLoc = glGetUniformLocation(fogShaderProgram, "view");
    if (viewLoc == -1) {
        debugMessages.push_back("View matrix uniform not found.");
    }
}

void CheckOpenGLError(const std::string& message) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        debugMessages.push_back(message + " OpenGL error: " + std::to_string(err));
    }
}

// Function to clean up the scene resources
void CleanupScene() {
    debugMessages.push_back("Cleaning up scene resources.");

    // Delete shader programs
    if (fogShaderProgram) {
        glDeleteProgram(fogShaderProgram);
        fogShaderProgram = 0;
    }

    if (arrowShaderProgram) {
        glDeleteProgram(arrowShaderProgram);
        arrowShaderProgram = 0;
    }

    // Delete VAO and VBO for the cube
    if (VAO) {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }

    if (VBO) {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }

    debugMessages.push_back("Scene resources cleaned up successfully.");
}