#ifndef BACKGROUND_VFX_H
#define BACKGROUND_VFX_H

#include <glm/glm.hpp>

// Structure for dust particles
struct DustParticle
{
    float x, y;      // Position
    float vx, vy;    // Velocity
    float size;      // Size of the particle
    float alpha;     // Transparency
    float lifespan;  // Remaining lifespan of the particle
    bool fadingIn;   // True if particle is fading in
};

// Structure for fog particles
struct FogParticle
{
    float x, y;      // Position
    float vx, vy;    // Velocity
    float size;      // Size of the particle
    float alpha;     // Transparency
};

// Function declarations
void InitDustParticles(int windowWidth, int windowHeight);
void InitFogParticles(int windowWidth, int windowHeight);
void UpdateDustParticles(float deltaTime, int windowWidth, int windowHeight);
void UpdateFogParticles(float deltaTime, int windowWidth, int windowHeight);
void RenderDustParticles(int windowWidth, int windowHeight);
void RenderFogParticles(int windowWidth, int windowHeight);
bool LoadFogTextures();  // New function to load both fog textures
bool LoadFogTexture(const char* filePath, GLuint& textureID);  // New function to load a specific texture
void RenderFogTextures(int windowWidth, int windowHeight, float deltaTime);  // New function to render fog textures
void RenderGradientBackground(int windowWidth, int windowHeight);

#endif // BACKGROUND_VFX_H
