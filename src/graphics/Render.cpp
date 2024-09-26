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

    // Initialize cube (VAO/VBO setup)
    //InitCube();

    // Initialize arrow (VAO/VBO setup)
    //InitArrow(camera.GetPosition(), camera.GetDirection());

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
    //glDisable(GL_CULL_FACE);  // Disable face culling

    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // Restore fill mode
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);  // Set background to gray

    // Get view and projection matrices from the camera
    //glm::mat4 view = camera.GetViewMatrix();
    //glm::mat4 projection = camera.GetProjectionMatrix(90.0f, 1280.0f / 720.0f, 0.1f, 100.0f);  // Set FOV to 90 degrees

    // Use the arrow shader and set up the view/projection matrices
    //glUseProgram(arrowShaderProgram);
    //glUniformMatrix4fv(glGetUniformLocation(arrowShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    //glUniformMatrix4fv(glGetUniformLocation(arrowShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Render the arrow (assumes arrow points from camera direction)
    //RenderArrow(arrowShaderProgram, camera.GetPosition(), camera.GetDirection());

    //glUseProgram(0);  // Stop using the shader after drawing

    // Use the fog shader program and set the view/projection matrices
    //debugMessages.push_back("Before using fogShaderProgram");
    //glUseProgram(fogShaderProgram);
    //CheckOpenGLError("After using fogShaderProgram");
    //glUniformMatrix4fv(glGetUniformLocation(fogShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    //glUniformMatrix4fv(glGetUniformLocation(fogShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    //CheckOpenGLError("OpenGL error after glUseProgram: ");
    
    // Render the cube
    //debugMessages.push_back("Before rendering cube");
    //glBindVertexArray(VAO);  // Bind the correct VAO before drawing
    //RenderCube(fogShaderProgram, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
    //CheckOpenGLError("After rendering cube");

    // Apply translation and scaling to the cube
    //glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));  // Move the cube closer to the camera
    //model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));  // Scale the cube down
    //glUniformMatrix4fv(glGetUniformLocation(fogShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    // Pass camera position to the fog shader
    //glUniform3f(glGetUniformLocation(fogShaderProgram, "cameraPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);



    /*
    // Render 100 cubes around the camera
    for (int i = 0; i < 100; i++) {
        glm::vec3 randomPosition = camera.GetPosition() + glm::vec3(
            (rand() % 20 - 10),   // Random x between -10 and 10
            (rand() % 20 - 10),   // Random y between -10 and 10
            (rand() % 20 - 10));  // Random z between -10 and 10

        glm::mat4 model = glm::translate(glm::mat4(1.0f), randomPosition);  // Move the cube to a random position
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));  // Scale the cube down
        glUniformMatrix4fv(glGetUniformLocation(fogShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

        // Render the cube
        RenderCube(fogShaderProgram, randomPosition.x, randomPosition.y, randomPosition.z);
    }
    */


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