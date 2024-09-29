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
#include "ui/Settings.h"          // Game settings management
#include "gameplay/Gameplay.h"          // Gameplay functionality
#include "ui/MainMenu.h"          // Main menu management
#include "ui/GUIStyle.h"          // GUI styling
#include "ui/Credits.h"           // Credits screen functionality
#include "ui/BackgroundVFX.h"     // Background visual effects
#include "graphics/ShaderUtils.h"       // Shader utilities for loading shaders
#include "utils/LogUtils.h"          // Custom logging utilities
#include "entities/SimpleCube.h"        // Rendering a simple cube
#include "camera/Camera.h"            // Camera functionality
#include "entities/SimpleArrow.h"       // Arrow rendering functionality
#include "gameplay/GameStateHandler.h"  // Game state management
#include "ui/SplashScreen.h"

// Custom modular headers
#include "graphics/Render.h"        // Rendering system
#include "input/Input.h"         // Input handling system
#include "ui/DebugUI.h"       // Debugging UI components
#include "camera/CameraManager.h" // Camera management
#include "models/ModelLoader.h"   // For model loading (e.g., FBX)
#include "core/CoreUI.h"        // Core UI elements (ImGui)
#include "graphics/GLUtils.h"       // OpenGL utilities and helpers
#include "audio/FMODManager.h"
#include "managers/ConfigManager.h"

#endif // INCLUDES_H
