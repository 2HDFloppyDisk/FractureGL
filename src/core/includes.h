#ifndef INCLUDES_H
#define INCLUDES_H

// Standard libraries
#include <windows.h>      // Only if you're targeting Windows-specific functionality
#include <vector>         // Standard library for container usage
#include <string>         // Standard library for string handling
#include <time.h>         // For time-based functionality

// OpenGL and related libraries
#include <GL/glew.h>      // GLEW (must be before OpenGL)
#include <GLFW/glfw3.h>   // GLFW for window and input handling
#include <glm/glm.hpp>    // GLM for vector/matrix operations
#include <glm/gtc/matrix_transform.hpp>  // GLM for matrix transformations
#include <glm/gtc/type_ptr.hpp>  // GLM for accessing matrix pointers

// ImGui libraries
#include <imgui/imgui.h>  
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

// Assimp for model loading
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Game-specific includes
#include "Settings.h"          // Game settings management
#include "Gameplay.h"          // Gameplay functionality
#include "MainMenu.h"          // Main menu management
#include "GUIStyle.h"          // GUI styling
#include "Credits.h"           // Credits screen functionality
#include "BackgroundVFX.h"     // Background visual effects
#include "ShaderUtils.h"       // Shader utilities for loading shaders
#include "LogUtils.h"          // Custom logging utilities
#include "SimpleCube.h"        // Rendering a simple cube
#include "Camera.h"            // Camera functionality
#include "SimpleArrow.h"       // Arrow rendering functionality
#include "GameStateHandler.h"  // Game state management

// Custom modular headers
#include "Render.h"        // Rendering system
#include "Input.h"         // Input handling system
#include "DebugUI.h"       // Debugging UI components
#include "CameraManager.h" // Camera management
#include "ModelLoader.h"   // For model loading (e.g., FBX)
#include "CoreUI.h"        // Core UI elements (ImGui)
#include "GLUtils.h"       // OpenGL utilities and helpers
#include "FMODManager.h"


#endif // INCLUDES_H
