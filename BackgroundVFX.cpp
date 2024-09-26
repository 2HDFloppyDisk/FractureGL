#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "BackgroundVFX.h"
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>    // For loading images (make sure stb_image.h is included in your project)



// Vectors to hold the dust, fog particles, and textures
std::vector<DustParticle> dustParticles;
std::vector<FogParticle> fogParticles;
GLuint fogTextureID1 = 0;
GLuint fogTextureID2 = 0;
float fogOffset1 = 0.0f;
float fogOffset2 = 0.0f;

// Function to initialize a single dust particle
DustParticle CreateDustParticle(int windowWidth, int windowHeight)
{
    DustParticle p;
    p.x = static_cast<float>(rand() % windowWidth);
    p.y = windowHeight + static_cast<float>(rand() % 50); // Start below the window
    p.vx = (static_cast<float>(rand() % 50) / 100.0f) + 0.05f; // Move slightly right
    p.vy = -(static_cast<float>(rand() % 50) / 100.0f) - 0.05f; // Move upwards
    p.size = 0.5f + static_cast<float>(rand() % 3);  // Smaller dust particle sizes between 1.5-3 pixels
    p.alpha = 0.01f; +static_cast<float>(rand() % 10) / 100.0f;  // Very low alpha for soft fog effect
    p.lifespan = 15.0f + static_cast<float>(rand() % 10); // Lifespan between 15-25 seconds
    p.fadingIn = false;  // Start particles in the fading-in state
    return p;
}

// Function to initialize a single fog particle
FogParticle CreateFogParticle(int windowWidth, int windowHeight)
{
    FogParticle p;
    p.x = static_cast<float>(rand() % windowWidth);
    p.y = static_cast<float>(rand() % windowHeight);
    p.vx = (static_cast<float>(rand() % 50) / 100.0f) + 2.05f; // Move slightly right
    p.vy = -(static_cast<float>(rand() % 50) / 100.0f) - 1.05f; // Move upwards
    p.size = 0.5f + static_cast<float>(rand() % 20) / 10.0f;  // Tiny fog particle sizes between 0.5-1 pixels
    p.alpha = 0.01f + static_cast<float>(rand() % 10) / 100.0f;  // Very low alpha for soft fog effect
    return p;
}

// Initialize the dust particle system
void InitDustParticles(int windowWidth, int windowHeight)
{
    dustParticles.clear();
    for (int i = 0; i < 500; ++i) // Create 500 dust particles for a denser effect
    {
        dustParticles.push_back(CreateDustParticle(windowWidth, windowHeight));
    }
}

// Initialize the fog particle system
void InitFogParticles(int windowWidth, int windowHeight)
{
    fogParticles.clear();
    for (int i = 0; i < 5000; ++i) // Create 5000 very small fog particles for dense fog
    {
        fogParticles.push_back(CreateFogParticle(windowWidth, windowHeight));
    }
}

// Update the dust particle system
void UpdateDustParticles(float deltaTime, int windowWidth, int windowHeight)
{
    for (auto& p : dustParticles)
    {
        p.x += p.vx * deltaTime * 50.0f; // Rightward movement
        p.y += p.vy * deltaTime * 50.0f; // Upward movement

        p.lifespan -= deltaTime; // Reduce lifespan

        // Handle fading in and out
        if (p.fadingIn)
        {
            p.alpha += deltaTime * 0.5f;
            if (p.alpha >= 1.0f)
            {
                p.alpha = 1.0f;
                p.fadingIn = false;
            }
        }
        else if (p.lifespan < 5.0f)  // Start fading out when lifespan is below 5 seconds
        {
            p.alpha = (p.lifespan / 5.0f);
        }

        // Respawn particles when they go off screen
        if (p.x > windowWidth || p.y < 0.0f || p.lifespan <= 0.0f)
        {
            p = CreateDustParticle(windowWidth, windowHeight); // Reset particle
        }
    }
}

// Update the fog particle system
void UpdateFogParticles(float deltaTime, int windowWidth, int windowHeight)
{
    for (auto& p : fogParticles)
    {
        p.x += p.vx * deltaTime * 10.0f; // Very slow horizontal movement
        p.y += p.vy * deltaTime * 10.0f; // Very slow upward movement

        // Respawn particles when they go off screen
        if (p.x > windowWidth || p.y < 0.0f)
        {
            p = CreateFogParticle(windowWidth, windowHeight); // Reset particle
        }
    }
}

// Render the dust particles
void RenderDustParticles(int windowWidth, int windowHeight)
{
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList(); // Draw in the background

    for (const auto& p : dustParticles)
    {
        ImColor color(63, 85, 89, static_cast<int>(p.alpha * 255));
        draw_list->AddCircleFilled(ImVec2(p.x, p.y), p.size, color);   // Draw particle as a small, semi-transparent circle
    }
}

// Render the fog particles
void RenderFogParticles(int windowWidth, int windowHeight)
{
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList(); // Draw in the background

    for (const auto& p : fogParticles)
    {
        ImColor color(63, 85, 89, static_cast<int>(p.alpha * 255)); // Set fog particle color with transparency
        draw_list->AddCircleFilled(ImVec2(p.x, p.y), p.size, color);   // Draw particle as a small, semi-transparent circle
    }
}

// Render a gradient background
void RenderGradientBackground(int windowWidth, int windowHeight)
{
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

    ImColor bottomColor = ImColor(90, 90, 90, 255);  // Lighter black (dark gray) at the top
    ImColor topColor = ImColor(0, 0, 0, 255);  // Black at the bottom

    // Render gradient rectangle
    draw_list->AddRectFilledMultiColor(
        ImVec2(-500, -500), ImVec2(windowWidth, windowHeight),
        topColor, bottomColor, topColor, topColor
    );
}
