#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

// Function to load and compile shaders
GLuint LoadShader(const char* vertexPath, const char* fragmentPath);

// New function to load all necessary shaders
void LoadShaders(GLuint& fogShaderProgram, GLuint& arrowShaderProgram);

#endif
