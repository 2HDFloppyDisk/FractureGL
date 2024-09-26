#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../entities/SimpleArrow.h"
#include "../models/ModelLoader.h"
#include "../utils/LogUtils.h"

void InitArrow(glm::vec3 cameraPos, glm::vec3 cameraFront) {
    // Generate and bind the arrow VAO
    glGenVertexArrays(1, &arrowVAO);
    glBindVertexArray(arrowVAO);

    // Generate and bind the VBO
    glGenBuffers(1, &arrowVBO);
    glBindBuffer(GL_ARRAY_BUFFER, arrowVBO);

    // Set up vertices for the arrow (a line from the camera position pointing in the camera's direction)
    float arrowLength = 2.0f;  // Increase the length of the arrow
    float arrowVertices[] = {
        cameraPos.x, cameraPos.y, cameraPos.z,  // Start point (camera position)
        cameraPos.x + cameraFront.x * arrowLength, cameraPos.y + cameraFront.y * arrowLength, cameraPos.z + cameraFront.z * arrowLength  // End point (longer)
    };

    // Load arrow vertices into the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(arrowVertices), arrowVertices, GL_STATIC_DRAW);

    // Define vertex attributes (position, etc.)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Unbind the VAO
    glBindVertexArray(0);

    // Optionally, add a debug message
    debugMessages.push_back("Arrow VAO initialized: " + std::to_string(arrowVAO));
}

// Function to render a directional arrow from the camera's position in its front direction
void RenderArrow(GLuint arrowShaderProgram, glm::vec3 cameraPos, glm::vec3 cameraFront) {

    glUseProgram(arrowShaderProgram);

    // Draw the arrow as a line
    glBindVertexArray(arrowVAO);
    glDrawArrays(GL_LINES, 0, 2);  // Draw 2 points as a line
    //glBindVertexArray(0);

    // Check if the correct VAO is bound
    int currentVAO;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVAO);
    debugMessages.push_back("Current VAO for arrow: " + std::to_string(currentVAO));

    // Cleanup
    //glDeleteVertexArrays(1, &arrowVAO);
    //glDeleteBuffers(1, &arrowVBO);
}
