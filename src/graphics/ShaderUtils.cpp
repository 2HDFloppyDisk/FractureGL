#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include "utils/LogUtils.h"
#include "graphics/ShaderUtils.h"


std::string LoadShaderSource(const char* filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        debugMessages.push_back("Failed to open shader file: " + std::string(filePath));
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint CompileShader(const char* shaderSource, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    // Check for compilation errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        debugMessages.push_back("Shader compilation failed: " + std::string(infoLog));
        return 0;
    }
    return shader;
}

GLuint LoadShader(const char* vertexPath, const char* fragmentPath) {
    // Load vertex and fragment shader sources
    std::string vertexCode = LoadShaderSource(vertexPath);
    std::string fragmentCode = LoadShaderSource(fragmentPath);

    if (vertexCode.empty() || fragmentCode.empty()) {
        debugMessages.push_back("Shader source is empty, could not load shaders.");
        return 0;
    }

    // Compile shaders
    GLuint vertexShader = CompileShader(vertexCode.c_str(), GL_VERTEX_SHADER);
    GLuint fragmentShader = CompileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);

    if (!vertexShader || !fragmentShader) {
        return 0;
    }

    // Link shaders into a program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        debugMessages.push_back("Shader linking failed: " + std::string(infoLog));
        return 0;
    }

    // Cleanup
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}
