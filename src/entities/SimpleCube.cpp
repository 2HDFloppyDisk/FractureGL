#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../entities/SimpleCube.h"
#include "../utils/LogUtils.h"

// Function to initialize the cube (run this once)
void InitCube() {
    debugMessages.push_back("Cube system initialized!");  // Add test message once
    // Vertex data for a cube (positions)
    float vertices[] = {
        // Positions          
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    };
    /*
    // Generate and bind VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    */

    // Generate and bind the VAO for the cube
    glGenVertexArrays(1, &VAO);  // VAO should be a global or accessible variable
    glBindVertexArray(VAO);      // Bind the VAO for setting up attributes

    // Generate and bind the VBO for the cube's vertices
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Upload the vertices data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Specify the layout of the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind the VAO (so it's not accidentally modified)
    glBindVertexArray(0);
}

// Function to render the cube
void RenderCube(GLuint fogShaderProgram, float cameraX, float cameraY, float cameraZ) {

    // Use the fog shader program
    glUseProgram(fogShaderProgram);


    glBindVertexArray(VAO);  // Bind VAO before drawing
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  // Bind the VBO if necessary

    // Check if the correct VAO is bound
    int currentVAO;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVAO);
    debugMessages.push_back("Current VAO for Cube: " + std::to_string(currentVAO));


    // Set up uniforms for the fog shader
    glUniform3f(glGetUniformLocation(fogShaderProgram, "cameraPos"), cameraX, cameraY, cameraZ);
    glUniform3f(glGetUniformLocation(fogShaderProgram, "fogColor"), 0.5f, 0.5f, 0.5f);  // Example fog color
    glUniform1f(glGetUniformLocation(fogShaderProgram, "fogDensity"), 0.1f);
    glUniform1f(glGetUniformLocation(fogShaderProgram, "fogHeight"), 1.0f);
    glUniform1f(glGetUniformLocation(fogShaderProgram, "scatteringCoefficient"), 0.02f);

    GLint modelLoc = glGetUniformLocation(fogShaderProgram, "model");
    if (modelLoc == -1) {
        debugMessages.push_back("Error: Could not find uniform location for model matrix");
    }


    // Bind the cube VAO and render
    //glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Unbind VAO after rendering
    glBindVertexArray(0);
}
